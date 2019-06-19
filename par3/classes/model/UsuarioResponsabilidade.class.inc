<?php
/**
 * Classe de mapeamento da entidade par3.usuarioresponsabilidade
*
* @category Class
* @package  A1
* @author   Eduardo Dunice <eduardoneto@mec.gov.br>
* @license  GNU simec.mec.gov.br
* @version  Release: 28-09-2015
* @link     no link
*/

/**
 * Par_Modelo_UsuarioResponsabilidade
*
* @category Class
* @package  A1
* @author   Eduardo Dunice <eduardoneto@mec.gov.br>
* @license  GNU simec.mec.gov.br
* @version  Release: 28-09-2015
* @link     no link 
*/
class Par3_Model_UsuarioResponsabilidade extends Modelo
{
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'par3.usuarioresponsabilidade';

	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array(
		'rpuid',
	);

	/**
	 * Chaves estrangeiras.
	 *
	 * @var array
	*/
	protected $arChaveEstrangeira = array(
		'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
	);

	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		'prgid' => null,
		'estuf' => null,
		'entid' => null,
		'muncod' => null,
		'rpudata_inc' => null,
		'rpustatus' => null,
		'usucpf' => null,
		'pflcod' => null,
		'rpuid' => null,
	);

	const ADMINISTRADOR			= 1445;
	const SUPER_USUARIO			= 1430;
	const SUPER_USUARIO_DESENVOLVEDOR = 1756;
	const EQUIPE_MUNICIPAL  	= 1431;
	const DIRIGENTE_MUNICIPAL   = 1433;
	const PREFEITO 				= 1436;
	const EQUIPE_ESTADUAL   	= 1432;
	const DIRIGENTE_ESTADUAL    = 1434;
	const NUTRICIONISTA 		= 1435;
	const SECRETARIO_ESTADUAL	= 1437;
	const CONSULTA_GERAL	    = 1447;
	const SUPORTE_MEC	        = 1468;
	const SUPORTE_FNDE          = 1469;
	const CONSELHEIRO           = 1477;
    const CONSULTA_ESTADUAL	    = 1480;
    const CONSULTA_MUNICIPAL	= 1481;
    const ANALISTA_PROGRAMAS	= 1479;
    const CONSELHEIRO_CACS	    = 1502;
    const PRESIDENTE_CACS	    = 1501;
    const CURSO_MEDICINA_AVAL_INST  = 1653;
    const COORDENADOR_TECNICO_CGEST = 1690;
    const ANALISTA_TECNICO_CGEST    = 1691;
    const PROFE_PROCURADOR_DIJUR    = 1692;
    const PROFE_ANALISTA_DIJUR      = 1693;
	const SECRETARIO_MUNICIPAL      = 1694;
	const ANALISTA_TECNICO_CGDEN    = 1798;
	const ANALISTA_PLANEJAMENTO     = 1771;
	const PAINEL                    = 1807;
	const UNIDADE_EXECUTORA         = 1862;
	const CONTROLE_USUARIO         = 1461;



    /**
	* Função validarUsuarioResponsabilidade
	* - valida se o CPF possui perfil para determinado municipio.
	*
	* @param string $usucpf
	* @param string $pflcod
	* @param string $muncod (optional)
	*
	* @return array.
	*
	*/
	public function validarUsuarioResponsabilidade($usucpf=null, $pflcod=null, $muncod = null, $estuf= null) {
		$where = array();

		if ($usucpf) $where[] = "rsp.usucpf = '{$usucpf}'";
		if ($pflcod) $where[] = "rsp.pflcod = '{$pflcod}'";
		if ($muncod) $where[] = "rsp.muncod = '{$muncod}'";
		if ($estuf)  $where[] = "rsp.estuf = '{$estuf}'";

		$where[] = "rsp.rpustatus = 'A'";
		$where[] = "usu.suscod = 'A'";
		$where[] = "uss.suscod = 'A'";
		$where[] = "uss.sisid = {$_SESSION['sisid']}";

		$sql = "SELECT rsp.rpuid, rsp.pflcod
				FROM par3.usuarioresponsabilidade rsp
				INNER JOIN seguranca.usuario usu ON rsp.usucpf = usu.usucpf
				INNER JOIN seguranca.usuario_sistema uss ON uss.usucpf = usu.usucpf
				WHERE ".implode(' AND ', $where);

		return $this->carregar($sql);
	}//end inativaEntidadesInuidPorTipo()


	/**
	 * Função validarUsuarioResponsabilidade
	 * - valida se o CPF possui perfil para determinado municipio.
	 *
	 * @param numeric $pflcod
	 * @param string  $muncod
	 * @param string  $estuf
	 *
	 * @return string.
	 *
	 */
	public function recuperarCPFUsuarioResponsabilidade($pflcod=null, $muncod = null, $estuf = null)
	{
	    $where = array();

	    $where[] = "usucpf = '{$_SESSION['usucpf']}'";
	    $where[] = "rpustatus = 'A'";

		if ($pflcod) $where[] = "pflcod = '{$pflcod}'";
		if ($muncod) $where[] = "muncod = '{$muncod}'";
		if ($estuf)  $where[] = "estuf = '{$estuf}'";

		$sql = "SELECT usucpf
				FROM par3.usuarioresponsabilidade
				WHERE ".implode(' AND ', $where);

		return $this->pegaUm($sql, 0, 60);
	}//end recuperarCPFUsuarioResponsabilidade()
	
	/**
	 * Função recuperarMunicipioUsuarioResponsabilidade
	 * - retorna os códigos de municípios viculados ao usuário.
	 *
	 * @param numeric $pflcod
	 * @param string  $muncod
	 * @param string  $estuf
	 *
	 * @return string.
	 *
	 */
	public function recuperarMunicipioUsuarioResponsabilidade($pflcod=null, $usucpf=null)
	{
		$pflcod = (array) $pflcod;
		$usucpf = ($usucpf ? $usucpf : $_SESSION['usucpf']);
		
	    $where = array();

	    $where[] = "usucpf = '{$usucpf}'";
	    $where[] = "rpustatus = 'A'";
	    $where[] = "muncod IS NOT NULL";

		if ($pflcod) $where[] = "pflcod IN ('". implode("', '", $pflcod) ."')";

		$sql = "SELECT muncod
				FROM par3.usuarioresponsabilidade
				WHERE ".implode(' AND ', $where);
		$dado = $this->carregarColuna($sql);
		
		return ($dado ? $dado : array());
	}//end recuperarMunicipioUsuarioResponsabilidade()


	/**
	 * Função possuiPerfil
	 * - serifica se possui algum dos pefils passados
	 *
	 * @param string $usucpf
	 *
	 * @return boolean.
	 *
	 */
	public function possuiPerfil($pflcod=array())
	{
	    $sql = "SELECT DISTINCT
    				TRUE
    			FROM
    				seguranca.perfilusuario
    			WHERE
    				usucpf = '{$_SESSION['usucpf']}'
    				AND pflcod IN (".implode(",", $pflcod).")";

		return $this->pegaUm($sql) == 't';
	}//end possuiPerfil()


	public function recuperaInuidUsuarioResponsabilidade($arrPost){

		$where =  Par3_Model_InstrumentoUnidade::prepararFiltro($arrPost);

        $sql = "SELECT
                    inuid
    	        FROM
    	            par3.instrumentounidade inu
                LEFT JOIN territorios.municipio mun ON mun.muncod = inu.muncod
                WHERE
    	           ".implode(' AND ', $where);

        return $this->pegaUm($sql);
	}

	/**
	 * Função retornaUsuarioResponsabilidade
	 * - valida se o CPF possui perfil para determinado municipio.
	 *
	 * @param string $usucpf
	 * @param string $pflcod
	 * @param string $muncod (optional)
	 *
	 * @return array.
	 *
	 */
	public function retornaUsuarioResponsabilidade($usucpf=null, $pflcod=null, $muncod = null, $estuf= null)
	{
		$where = array();

		$where[] = "rpu.rpustatus = 'A'";
		$where[] = "usu.suscod = 'A'";

		if ($usucpf) $where[] = "rpu.usucpf = '{$usucpf}'";
		if ($pflcod) $where[] = "rpu.pflcod = '{$pflcod}'";
		if ($muncod) $where[] = "rpu.muncod = '{$muncod}'";
		if ($estuf)  $where[] = "rpu.estuf = '{$estuf}'";

		$sql = "SELECT rpu.*
                FROM par3.usuarioresponsabilidade rpu
                INNER JOIN seguranca.usuario_sistema 	usi ON usi.usucpf = rpu.usucpf AND suscod = 'A'
                INNER JOIN seguranca.perfilusuario 	    peu ON peu.pflcod = rpu.pflcod AND peu.usucpf = rpu.usucpf
				INNER JOIN seguranca.usuario 			usu ON rpu.usucpf = usu.usucpf
				WHERE ".implode(' AND ', $where);

		return $this->pegaLinha($sql);
	}//end inativaEntidadesInuidPorTipo()

    /**
     * Força permissão apenas de leitura para perfis de consulta[geral, municipal, estadual]
     * @param $disabled
     * @param $disabledForm
     * @return string
     */
    public static function permissaoEscrita($disabled, $disabledForm)
    {
        global $arrPermissao;
        is_null($disabled) && $disabled = 'disabled="disabled"';

        if (!$arrPermissao['booEscrita']) {
            $attr = $disabled;
        } else if ($arrPermissao['booEscrita'])  {
            $attr = '';
        } else {
            $attr = is_array($disabledForm) ? '' : $disabled;
        }

        //ver($attr, $arrPermissao, d);
        return $attr;
    }


    public static function perfilGestorUnidade()
    {
        $instance = new self();
        if ($instance->possuiPerfil([
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
            Par3_Model_UsuarioResponsabilidade::PREFEITO,
            Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL,
            Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
            Par3_Model_UsuarioResponsabilidade::SUPORTE_FNDE,
            Par3_Model_UsuarioResponsabilidade::SUPORTE_MEC,
            Par3_Model_UsuarioResponsabilidade::ANALISTA_PLANEJAMENTO,
        ])) {
            return true;
        } else {
            return false;
        }
    }

    public static function perfilFinalizarPlanejamento()
    {
        $instance = new self();
        if ($instance->possuiPerfil([
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::PREFEITO,
            Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL
        ])) {
            return true;
        } else {
            return false;
        }
    }

    public static function perfilConsulta()
    {
        $instance = new self();
        if ($instance->possuiPerfil([
            Par3_Model_UsuarioResponsabilidade::CONSULTA_GERAL,
            Par3_Model_UsuarioResponsabilidade::CONSULTA_MUNICIPAL,
            Par3_Model_UsuarioResponsabilidade::CONSULTA_ESTADUAL,
            Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS,
            Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS
        ])) {
            return true;
        } else {
            return false;
        }
    }

	function salvarUsuarioResponsabilidade($dados, $pflcods, $table = '')
    {

    	$salvaPar3 = (empty($table) ? true : false);
        $table = (empty($table) ? $this->stNomeTabela : $table);
        foreach ($pflcods as $pflcod) {
            foreach ($dados['esfera'] as $keyEsfera => $esfera) {

                if ($esfera == 'ESTADUAL') {

                    $existeUsuarioResp = $this->existeUsuarioRespCarga($dados['usucpf'], $pflcod, null, $dados['estuf'][$keyEsfera], $table);
                    if (!$existeUsuarioResp) {

						if($salvaPar3){
							$estuf = $dados['estuf'][0];

                            $sqlEntidade = "SELECT DISTINCT
								e.entid as codigo,
								e.entnome || ' - ' || ed.estuf as descricao,
								ef.funid,
								ed.estuf
							FROM
								entidade.entidade e
							INNER JOIN entidade.funcaoentidade 		ef ON ef.entid = e.entid
							LEFT  JOIN entidade.endereco 			ed ON ed.entid = e.entid
							WHERE
								ef.funid in (6) AND
								ed.estuf = '$estuf'
								AND e.entstatus = 'A'";
							$arrEntidade = $this->pegaLinha($sqlEntidade);

							$codEntidade = ( !empty($arrEntidade['codigo']) ? " '{$arrEntidade['codigo']}' " : 'null');

                            $sql = "INSERT INTO {$table} (usucpf, rpustatus, muncod,estuf, pflcod, rpudata_inc, entid)
    								VALUES ('{$dados['usucpf']}', 'A', null,'{$dados['estuf'][$keyEsfera]}', {$pflcod}, 'now()', {$codEntidade}) returning rpuid;";

						} else {
                            $sql = "INSERT INTO {$table} (usucpf, rpustatus, muncod,estuf, pflcod, rpudata_inc)
    							VALUES ('{$dados['usucpf']}', 'A', null,'{$dados['estuf'][$keyEsfera]}', {$pflcod}, 'now()') returning rpuid;";
						}

                        return $this->executar($sql, false);
                    }

                } else {

                    $existeUsuarioResp = $this->existeUsuarioRespCarga($dados['usucpf'], $pflcod, $dados['muncod'][$keyEsfera], null, $table);

                    if (!$existeUsuarioResp) {

                    	if($salvaPar3) {
                            $muncod = $dados['muncod'][0];

                            $sqlEntidade = "SELECT DISTINCT
                        	e.entid as codigo,
                        	CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
                        		e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
                        		e.entnome END as descricao,
                        	ef.funid,
                        	m.muncod
                        FROM
                        	entidade.entidade e
                        INNER JOIN entidade.funcaoentidade 		ef ON ef.entid = e.entid
                        LEFT  JOIN entidade.endereco 			ed ON ed.entid = e.entid
                        LEFT  JOIN territorios.municipio 		m  ON m.muncod = ed.muncod
                        WHERE
                        	ef.funid in (1)
                        	AND m.muncod = '$muncod'
        					AND e.entstatus = 'A'";
                            $arrEntidade = $this->pegaLinha($sqlEntidade);
                            $codEntidade = (!empty($arrEntidade['codigo']) ? " '{$arrEntidade['codigo']}' " : 'null');

                            $sql = "INSERT INTO {$table} (usucpf, rpustatus, muncod, estuf, pflcod, rpudata_inc, entid)
    							VALUES ('{$dados['usucpf']}', 'A', '{$dados['muncod'][$keyEsfera]}', null, {$pflcod}, 'now()', {$codEntidade}) returning rpuid;";

                        } else {
                            $sql = "INSERT INTO {$table} (usucpf, rpustatus, muncod, estuf, pflcod, rpudata_inc)
    							VALUES ('{$dados['usucpf']}', 'A', '{$dados['muncod'][$keyEsfera]}', null, {$pflcod}, 'now()') returning rpuid;";
						}

                        return $this->executar($sql, false);
                    }
                }
            }
        }
    }


        public function buscarDadosParaHistoricoUsuario( $dados ){
           global $db;

            if( $dados['pflcod'] == Par3_Model_UsuarioResponsabilidade::CONSELHEIRO ){
                $_resp['perfil'] = 'CONSELHEIRO';
            }

            if( $dados['pflcod'] == Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA ){
                $_resp['perfil'] = 'NUTRICIONISTA';
            }

            $sql = "
                SELECT  p.pfldsc
                FROM seguranca.perfilusuario pu
                JOIN seguranca.perfil p on p.pflcod = pu.pflcod
                WHERE p.pflstatus = 'A' AND p.sisid = 231 and pu.usucpf = '{$_SESSION['usucpf']}'
            ";
            $_resp['pf_resp'] = $this->pegaUm( $sql );

            if( $dados['muncod'] != '' ){
                $sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$dados['muncod']}'";
                $_resp['local'] = $db->pegaUm( $sql );
            }

            if( $dados['estuf'] != '' ){
                $sql = "SELECT estuf||' - '||estdescricao AS estado FROM territorios.estado WHERE estuf = '{$dados['estuf']}'";
                $_resp['local'] = $db->pegaUm( $sql );
            }

            $_resp['nome'] = $db->pegaUm("SELECT usunome FROM seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'");

            $_resp['hoje'] = date('d/m/Y H:i');

            return $_resp;
        }

        /**
         * Inativa o UsuarioResponsabilidade
         *
         */
        public function inativaUsuarioResponsabilidadeRotina($arrDados)
        {

        	/** CARREGA AS VARIÁVEIS NECESSÁRIAS*/
        	$codCargo = $arrDados['co_tp_entidade'];
        	/** PEGA O ID DO PERFIL DE ACORDO COM O CÓDIGO do CARGO */
        	$arrPerfis = array(
        			'02' 	=> PAR3_PERFIL_PREFEITO,
        			'10' 	=> PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,
        			'28' 	=> PAR3_PERFIL_DIRIGENTE_MUNICIPAL
        	);

        	/** PEGA O ID DO PERFIL  */
        	$idPerfil = $arrPerfis[$codCargo];

        	// Busca o inuid do par
        	if( ($codCargo == '02') || ($codCargo == '28') )
        	{
        		$muncod = $arrDados['muncod'];
        		$sql = "UPDATE par3.usuarioresponsabilidade SET rpustatus = 'I' WHERE muncod = '{$muncod}' and pflcod = {$idPerfil} ";

        	}
        	else if($codCargo == '10')
        	{
        		$estuf = $arrDados['estuf'];
        		$sql = "UPDATE par3.usuarioresponsabilidade SET rpustatus = 'I' WHERE estuf = '{$estuf}' and pflcod = {$idPerfil}";

        	}

        	/** EXECUTA */
        	if( $this->executar($sql) )
        	{
        		return true;
        	}
        }

        /**
         * Inativa o UsuarioResponsabilidade
         *
         */
        public function insereUsuarioResponsabilidadeRotina($arrDados)
        {

        	/** CARREGA AS VARIÁVEIS NECESSÁRIAS*/
        	$codCargo = $arrDados['co_tp_entidade'];
        	$usucpf		= $arrDados['entcpf'];
        	/** PEGA O ID DO PERFIL DE ACORDO COM O CÓDIGO do CARGO */
        	$arrPerfis = array(
        			'02' 	=> PAR3_PERFIL_PREFEITO,
        			'10' 	=> PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,
        			'28' 	=> PAR3_PERFIL_DIRIGENTE_MUNICIPAL
        	);

        	/** PEGA O ID DO PERFIL */
        	$idPerfil = $arrPerfis[$codCargo];

        	// Busca o inuid do par
        	if( ($codCargo == '02') || ($codCargo == '28') )
        	{
        		$muncod = $arrDados['muncod'];

        		$sql = "SELECT DISTINCT
                        	e.entid as codigo,
                        	CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
                        		e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
                        		e.entnome END as descricao,
                        	ef.funid,
                        	m.muncod
                        FROM
                        	entidade.entidade e
                        INNER JOIN entidade.funcaoentidade 		ef ON ef.entid = e.entid
                        LEFT  JOIN entidade.endereco 			ed ON ed.entid = e.entid
                        LEFT  JOIN territorios.municipio 		m  ON m.muncod = ed.muncod
                        WHERE
                        	ef.funid in (1)
                        	AND m.muncod = '$muncod'
        					AND e.entstatus = 'A'
        		";
        		$arrEntidade = $this->pegaLinha($sql);
                $codEntidade = ( !empty($arrEntidade['codigo']) ? " '{$arrEntidade['codigo']}' " : 'null');
        		$sql = "INSERT INTO par3.usuarioresponsabilidade
							(pflcod, usucpf, rpustatus, rpudata_inc, muncod, entid)
						VALUES
							({$idPerfil}, '{$usucpf}', 'A', now(), '{$muncod}', {$codEntidade})";

        	}
        	else if($codCargo == '10')
        	{
        		$estuf = $arrDados['estuf'];

        		$sql = "SELECT DISTINCT
                    		e.entid as codigo,
                    		e.entnome || ' - ' || ed.estuf as descricao,
                    		ef.funid,
                    		ed.estuf
                    	FROM
                    		entidade.entidade e
                    	INNER JOIN entidade.funcaoentidade 		ef ON ef.entid = e.entid
                    	LEFT  JOIN entidade.endereco 			ed ON ed.entid = e.entid
                    	WHERE
                    		ef.funid in (6) AND
        		            ed.estuf = '$estuf'
        					AND e.entstatus = 'A'
        		";
        		$arrEntidade = $this->pegaLinha($sql);

                $codEntidade = ( !empty($arrEntidade['codigo']) ? " '{$arrEntidade['codigo']}' " : 'null');
        		$sql = "INSERT INTO par3.usuarioresponsabilidade
							(pflcod, usucpf, rpustatus, rpudata_inc, estuf, entid)
						VALUES
							({$idPerfil}, '{$usucpf}', 'A', now(), '{$estuf}', {$codEntidade})";

        	}

        	/** EXECUTA */
        	if( $this->executar($sql) )
        	{
        		return true;
        	}
        }


    public function existeUsuarioRespCarga($usucpf=null, $pflcod=null, $muncod = null, $estuf= null, $table){
        $where = array();

        $where[] = "rpu.rpustatus = 'A'";

        if ($usucpf) $where[] = "rpu.usucpf = '{$usucpf}'";
        if ($pflcod) $where[] = "rpu.pflcod = '{$pflcod}'";
        if ($muncod) $where[] = "rpu.muncod = '{$muncod}'";
        if ($estuf)  $where[] = "rpu.estuf = '{$estuf}'";

        $sql = "SELECT rpu.rpuid
                FROM {$table} rpu
				WHERE ".implode(' AND ', $where);

        return $this->pegaUm($sql);
    }

    public function listaSqlPerfis()
	{
        $sqlPerfilAssociado = "select 
										pflcod as codigo,
										pfldsc as descricao 
									from 
										seguranca.perfil 
									where 
										pflstatus='A'
									and 
										sisid=".PAR3_SIS_ID."
									order by
										pfldsc ";
        return $sqlPerfilAssociado;
	}
}//end Class
?>
