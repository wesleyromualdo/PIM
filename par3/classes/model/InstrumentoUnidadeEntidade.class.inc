<?php
/**
 * Classe de mapeamento da entidade par3.instrumentounidade_entidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-09-2015
 * @link     no link
 */

/**
 * Par_Modelo_InstrumentoUnidadeEntidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-09-2015
 * @link     no link
 */
class Par3_Model_InstrumentoUnidadeEntidade extends Modelo
{
    /**
     * Nome da tabela especificada
     *
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.instrumentounidade_entidade';

    /**
     * Chave primaria.
     *
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'entid',
    );

    /**
     * Chaves estrangeiras.
     *
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'endid' => array('tabela' => 'par3.endereco', 'pk' => 'endid'),
        'tenid' => array('tabela' => 'par3.tipoentidade', 'pk' => 'tenid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * Atributos
     *
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'entid' => null,
        'inuid' => null,
        'tenid' => null,
        'endid' => null,
        'entcpf' => null,
        'entnome' => null,
        'entsexo' => null,
        'entemail' => null,
        'enttelefonecomercial' => null,
        'enttelefonecelular' => null,
    	'enttelefonefax' => null,
    	'entcnpj' => null,
        'entinscricaoestadual' => null,
        'entrazaosocial' => null,
        'entsigla' => null,
    	'entstatus' => null,
    	'entrg' => null,
    	'entorgexpedidor' => null,
    	'entdtnascimento' => null,
    	'entnivelensino' => null,
    	'entempoatuacao' => null,
    	'entcursomec' => null,
    	'entcursomecdescricao' => null,
        'entdtcriacao' => null,
        'entdtinativacao' => null,
        'entusucpfinativacao' => null,
        'entusucpfinclusao' => null,
        'entatuacao' => null,
        'entcargo' => null,
        'entdt_inicio_mandato' => null,
        'entdt_fim_mandato' => null,

    );
//INSERT INTO par3.tipoentidade(tendsc,tenunico) values('Conselho Infantil','f');

    const PREFEITURA                     = 1;
    const PREFEITO                       = 2;
    const SECRETARIA_EDUCACAO            = 3;
    const DIRIGENTE                      = 4;
    const EQUIPE                         = 5;
    const SECRETARIO_ESTADUAL_EDUCACAO   = 9;
    const NUTRICIONISTA_RESPONSAVEL      = 7;
    const NUTRICIONISTA_QUADRO           = 8;
    const CONSELHO_ESTADUAL	             = 10;
    const CONSELHO_MUNICIPAL	         = 11;
    const CONSELHO_CAE	                 = 12;
    const CONSELHO_CONSELHEIRO_CAE       = 13;
    const CONSELHO_PRESIDENTE_CAE        = 14;
    const EQUIPE_TECNICA                 = 15;
    const CONSELHO_CONSELHEIRO_INFANTIL  = 16;

    /**
     * Função pegarEntidAtivoPorTipo
     * - retorna o entid ativo.
     *
     * @return integer entid.
     *
     */
    public function pegarEntidAtivoPorTipo($inuid, $tenid)
    {
        if($inuid != ''){
        	$sql = "SELECT entid
        	        FROM {$this->stNomeTabela}
        	        WHERE
        	           inuid = $inuid
            	       AND entstatus = 'A'
            	       AND tenid = $tenid AND entnome IS NOT NULL";
      //     ver($sql,d);
        	return $this->pegaUm($sql, 0, 3600);


        }
    }//end pegarEntidAtivoPorTipo()


    /**
     * Função carregarDadosEntidPorTipo
     * - retorna o dados da entidade por id.
     *
     * @return retorna um objeto dirigente.
     *
     */
    public function carregarDadosEntidPorTipo($inuid, $tenid)
    {
    	$entid = $this->pegarEntidAtivoPorTipo($inuid, $tenid);
		$objEntidade = new Par3_Model_InstrumentoUnidadeEntidade($entid);

        return $objEntidade;
    }//end carregarDadosEntidPorTipo()

    public function carregarListaEntPorTipo($arrDados)
    {
        $sql = $this->montarSQLSimplesEntidades($arrDados);
       return $unidadeEntidades = $this->carregar($sql, 0, 30);
    }//end carregarListaEntPorTipo()

    /**
     * Função carregaArrayHistoricoEntidade
     * - retorna array de entidades.
     *
     * @return array $arrDados.
     *
     */
    public function carregaArrayHistoricoEntidade($arrPost)
    {
        $sql = self::pegarSQLHistoricoEntidade($arrPost);

        $arrDados = self::carregar($sql);

        return $arrDados;

    }//end carregaArrayHistoricoEntidade()

    /**
     * Função pegarSQLHistóricoEntidade
     * - retorna sql para histórico de entidade.
     *
     * @return string $sql.
     *
     */
    public function pegarSQLHistoricoEntidade($arrPost)
    {
        $where = self::montarFiltroHistoricoEntidade($arrPost);

        $sql = "SELECT	coalesce(entcpf, entcnpj) as id, *
                FROM par3.instrumentounidade_entidade ent
                LEFT  JOIN seguranca.usuario usu ON usu.usucpf = ent.entusucpfinativacao OR usu.usucpf = ent.entusucpfinclusao
                WHERE ".implode(' AND ', $where)."
                ORDER BY entdtinativacao";

        return $sql;

    }//end pegarSQLHistoricoEntidade()


    /**
     * Função montarFiltroHistóricoEntidade
     * - retorna filtro para histórico de entidade.
     *
     * @return string $sql.
     *
     */
    public function montarFiltroHistoricoEntidade($arrPost)
    {
        $where = array('entcpf IS NOT NULL');
        if ($arrPost['inuid'] != '') $where[]     = "inuid = {$arrPost['inuid']}";
        if ($arrPost['tenid'] != '') $where[]     = "tenid IN ({$arrPost['tenid']})";
        if ($arrPost['entstatus'] != '') $where[] = "entstatus = '{$arrPost['entstatus']}'";

        return $where;

    }//end montarFiltroHistoricoEntidade()


    /**
     * Função inativaEntidadesInuidPorTipo
     * - intaiva entidades da unidade por tipo.
     *
     * @return void.
     *
     */
    public function inativaEntidadesInuidPorTipo($inuid, $tenid)
    {
        $sql = "UPDATE 	{$this->stNomeTabela} SET
                    	entstatus = 'I',
                    	entdtinativacao = now(),
                    	entusucpfinativacao = '{$_SESSION['usucpf']}'
                WHERE 	inuid = $inuid
                  AND	tenid = $tenid";

        $this->executar($sql);
    }//end inativaEntidadesInuidPorTipo()


    /**
     * Função inativaEntidadesInuidPorId
     * - intaiva entidades da unidade por entid.
     *
     * @return void.
     *
     */
    public function inativaEntidadesInuidPorId($inuid, $entid)
    {
        $sql = "UPDATE 	{$this->stNomeTabela} SET
                    	entstatus = 'I',
                    	entdtinativacao = now(),
                    	entusucpfinativacao = '{$_SESSION['usucpf']}'
                WHERE 	inuid = $inuid
                  AND	entid = $entid";

        $this->executar($sql);
    }//end inativaEntidadesInuidId()
    /**
     * Função recuperarEntidadeUnidadePorCPF
     * - recupera uma pessoa fisica especifica da unidade
     *
     * @param integer       $inuid  código da unidade
     * @param character(11) $usucpf CPF da entidade
     * @param integer       $tenid  tipo da pessoa
     *
     * @return integer      $entid  id da entidade
     *
     */
    public function recuperarEntidadeUnidadePorCPF($inuid, $usucpf, $tenid){
        $usucpf = str_replace('.', '', str_replace('-', '', $usucpf) );

        $sql = "
            SELECT entid
            FROM {$this->stNomeTabela}  int
            INNER JOIN par3.instrumentounidade ine on int.inuid = ine.inuid

            WHERE int.entcpf = '{$usucpf}' AND int.inuid = {$inuid} AND int.entstatus = 'A' AND int.tenid = {$tenid}
        ";
        return $this->pegaUm($sql);
    }// end recuperarEntidadeUnidadePorCPF()

    public function recuperarEntidadePorCPF($inuid, $usucpf, $tenid,$entstatus = 'A'){
        $usucpf = str_replace('.', '', str_replace('-', '', $usucpf) );

        $sql = "
            SELECT *
            FROM {$this->stNomeTabela}  int
            INNER JOIN par3.instrumentounidade ine on int.inuid = ine.inuid
            WHERE int.entcpf = '{$usucpf}' AND int.inuid = {$inuid} AND int.entstatus = '{$entstatus}' AND int.tenid = {$tenid}
        ";
        return $this->recuperar($sql);
    }// end recuperarEntidadeUnidadePorCPF()
    /**
     * Função recuperarEntidadeUnidadePorEmail
     * - recupera uma pessoa fisica especifica da unidade
     *
     * @param integer       $inuid  código da unidade
     * @param character(11) $usucpf CPF da entidade
     * @param integer       $tenid  tipo da pessoa
     *
     * @return integer      $entid  id da entidade
     *
     */
    public function recuperarEntidadeUnidadePorEmail($entemail,$inuid, $tenid){
        $sql = "
            SELECT entid
            FROM {$this->stNomeTabela}  int
            INNER JOIN par3.instrumentounidade ine on int.inuid = ine.inuid

            WHERE int.entemail = '{$entemail}' AND int.inuid = {$inuid} AND int.entstatus = 'A' AND int.tenid = {$tenid}
        ";
        return $this->pegaUm($sql);
    }// end recuperarEntidadeUnidadePorCPF()

    public function recuperarNutricionistaResponsavelPorCpf($usucpf)
    {
        $sql = "
                select usunome,entemail,inudescricao,estuf,entemail,'instrumentounidade_entidade' as origem
                from {$this->stNomeTabela}  int
                join seguranca.usuario usu on int.entcpf = usu.usucpf
                join par3.instrumentounidade ine on int.inuid = ine.inuid
                where int.entcpf = '{$usucpf}'  and int.entstatus = 'A' and int.tenid = ".self::NUTRICIONISTA_RESPONSAVEL;

        return $this->carregar($sql);
    }

    /**
     * Função recuperarNutricionista
     * - recupera  o registro de um nitricionista
     * @param  array $arrPost
     * @return string $sql
     *
     */
    public function recuperarNutricionista($arrPost)
    {
        $sql = "
                select int.entid, entnome,entemail,int.enttelefonecomercial,int.tenid ,int.entcpf,tendsc
                from {$this->stNomeTabela}  int
                join seguranca.usuario       usu on int.entcpf = usu.usucpf
				INNER JOIN par3.tipoentidade ten ON ten.tenid  = int.tenid
                join par3.instrumentounidade ine on int.inuid = ine.inuid
                AND int.inuid = {$arrPost['inuid']}
                where int.entid = '{$arrPost['entid']}'  and int.entstatus = 'A' AND int.tenid IN (".self::NUTRICIONISTA_QUADRO.", ".self::NUTRICIONISTA_RESPONSAVEL.", ".self::CONSELHO_CONSELHEIRO_INFANTIL.")";

        return $this->pegaLinha($sql,0,60);
    }

    public function recuperarIntegranteEquipeTecnica($arrPost)
    {
        $sql = "
                select int.entid, entnome,entemail,int.enttelefonecomercial,int.tenid ,int.entcpf,tendsc
                from {$this->stNomeTabela}  int
                join seguranca.usuario       usu on int.entcpf = usu.usucpf
				INNER JOIN par3.tipoentidade ten ON ten.tenid  = int.tenid
                join par3.instrumentounidade ine on int.inuid = ine.inuid
                AND int.inuid = {$arrPost['inuid']}
                where int.entid = '{$arrPost['entid']}'  and int.entstatus = 'A' AND int.tenid = 15";

        return $this->pegaLinha($sql,0,60);
    }

    /**
     * Função updateTipoNutricionistaResponsavel
     * - Deve ser executado quando
     * @param  array $arrPost
     * @return string $sql
     *
     */
    public function updateTipoNutricionistaRT($inuid)
    {
        $sql = "
        UPDATE
            par3.instrumentounidade_entidade
        SET
            tenid     = ".self::NUTRICIONISTA_QUADRO."
        WHERE
            tenid = ".self::NUTRICIONISTA_RESPONSAVEL."
        AND
            inuid  = {$inuid};

        UPDATE
            par3.vinculacaonutricionista
        SET
            tenid     = ".self::NUTRICIONISTA_QUADRO."
        WHERE
            tenid = ".self::NUTRICIONISTA_RESPONSAVEL."
        AND
            inuid  = {$inuid}";
        $this->executar($sql);
        $this->commit();
    }

    public function verificarRTNutricionista($inuid)
    {
        $sql = "SELECT entid
            	FROM {$this->stNomeTabela}
            	WHERE tenid  = ".self::NUTRICIONISTA_RESPONSAVEL."
            	      AND inuid = {$inuid}
            	      AND entstatus = 'A'";
        return $this->pegaLinha($sql);
    }

    public function pegarSQLNutricionistasQuadroTecnico($arrPost)
    {
    	$sql = "SELECT int.entid as id,entcpf, entnome,tendsc, entemail, sndescricao
            	FROM {$this->stNomeTabela}  int
            	INNER JOIN par3.tipoentidade             ten ON ten.tenid  = int.tenid
            	INNER JOIN seguranca.usuario             usu ON int.entcpf = usu.usucpf
            	INNER JOIN par3.instrumentounidade       ine ON int.inuid  = ine.inuid
            	INNER JOIN par3.vinculacaonutricionista  vn  ON vn.tenid   = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND vn.vnstatus = 'A'
            	INNER JOIN par3.situacaonutricionista    sn  ON sn.snid    = vn.snid
            	WHERE
            	   AND int.inuid = {$arrPost['inuid']}
            	   AND int.entstatus = 'A'";
    	return $sql;
    }

    public function pegarSQLNutricionistas($arrPost)
    {
        $sql = "SELECT int.entid as id,entcpf, entnome, entemail,enttelefonecomercial,tendsc, sndescricao,danid
            	FROM {$this->stNomeTabela}  int
            	INNER JOIN par3.tipoentidade             ten ON ten.tenid  = int.tenid
            	INNER JOIN seguranca.usuario             usu ON int.entcpf = usu.usucpf
            	INNER JOIN par3.instrumentounidade       ine ON int.inuid  = ine.inuid
            	INNER JOIN par3.vinculacaonutricionista  vn  ON vn.tenid   = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND vn.vnstatus = 'A'
            	INNER JOIN par3.situacaonutricionista    sn  ON sn.snid    = vn.snid
            	left join  par3.dadosnutricionista       dan ON dan.dancpf = vn.vncpf
            	WHERE
            	   int.tenid IN (".self::NUTRICIONISTA_QUADRO.", ".self::NUTRICIONISTA_RESPONSAVEL.",".self::CONSELHO_CONSELHEIRO_INFANTIL.")
            	   AND int.inuid = {$arrPost['inuid']}
            	   AND int.entstatus = 'A'
            	   ORDER BY tendsc DESC";
        return $sql;
    }

	/**
	 * recupera um array com nutricionistas cadastrados
	 */
    public function recuperarNutricionistasQuadroTecnico()
    {
    	$sql = "
    	select int.entid as id,entcpf, entnome, entemail
    	from {$this->stNomeTabela}  int
    	join seguranca.usuario usu on int.entcpf = usu.usucpf
    	join par3.instrumentounidade ine on int.inuid = ine.inuid
    	where int.tenid = ".self::NUTRICIONISTA_QUADRO;

    	return $this->carregar($sql);
    }

    /**
     * recupera um array com nutricionistas cadastrados
     */
    public function existeConselheiro($usucpf, $inuid, $tenid)
    {
    	$sql = "SELECT int.entid as id
            	FROM {$this->stNomeTabela}  int
            	INNER JOIN seguranca.usuario       usu ON int.entcpf = usu.usucpf
            	INNER JOIN par3.instrumentounidade ine ON int.inuid = ine.inuid
            	WHERE
            	   int.tenid = ".$tenid."
            	   AND ine.inuid = {$inuid}
            	   and int.entcpf = '{$usucpf}'
            	   AND entstatus = 'A'";

    	return $this->pegaUm($sql);
    }


    /**
     * Função carregarConselheiros
     * - retorna array de entidades do tipo conselheiro.
     *
     * @return array $arrDados.
     *
     */
    public function carregarConselheiros($inuid, $tenid)
    {
        $sql = "SELECT ent.entid as id,entcpf, entnome, entemail, entatuacao, entcargo
                FROM {$this->stNomeTabela}  ent
                INNER JOIN par3.instrumentounidade  inu ON inu.inuid  = ent.inuid
                WHERE
                    ent.tenid = {$tenid}
                    AND inu.inuid = {$inuid}
                    AND ent.entstatus = 'A'";
        return $this->carregar($sql, 0, 30);
    }//end carregarConselheiros()


    /**
     * Função montarSQLEntidades
     * - retorna array de entidades simples.
     *
     * @return array $arrDados.
     *
     */
    public function montarSQLSimplesEntidades(array $arrPost){
    	$sql = sprintf('
            SELECT
              ent.entid as id,
              entcpf,
              entnome,
              enttelefonecomercial,
              CASE
                WHEN con.cacfuncao = \'P\'
                  THEN \'Presidente\'
                WHEN con.cacfuncao = \'V\'
                  THEN \'Vice Presidente\'
                WHEN con.cacfuncao = \'M\'
                  THEN \'Membro\'
              END AS cacfuncao,
              CASE
                WHEN con.cactipoocupacao = \'T\'
                  THEN \'Titular\'
                WHEN con.cactipoocupacao = \'S\'
                  THEN \'Suplente\'
              END AS cactipoocupacao,
              ccc.cctdsc,
              entemail,
              MAX(con.arqid) AS arqid
            FROM
              par3.instrumentounidade_entidade ent
              INNER JOIN par3.instrumentounidade inu
                ON inu.inuid = ent.inuid
              LEFT JOIN par3.cae_conselheiro con
                ON con.entid = ent.entid
              LEFT JOIN par3.cae_conselheiro_categoria ccc
                ON con.cctid = ccc.cctid
            WHERE
              ent.tenid = %d
              AND inu.inuid = %d
              AND ent.entstatus = \'A\'
            GROUP BY 
              ent.entid, 
              con.cacfuncao, 
              con.cactipoocupacao, 
              ccc.cctdsc
            ',
            $arrPost['tenid'],
            $arrPost['inuid']
        );

    	return $sql;
    }

    public function carregarDadosPorCpf($cpf,$inuid)
    {
        $dados = $this->pegaLinha("select * from par3.instrumentounidade_entidade where entcpf = '{$cpf}' and inuid = '{$inuid}'");
        return $dados;
    }

    public function contarEntidadesSQL($inuid,$tenid,$status)
    {
        $sql = "SELECT count(*) FROM par3.instrumentounidade_entidade  WHERE inuid = $inuid  AND tenid = $tenid AND entstatus = '{$status}'";
        return $sql;
    }

    /**
     * Método para verificar a consistência entre o cadastro do dirigente municipal
     * informado na unidade e o cadastrado no módulo do PAR 3 pela gestão de usuários
     *
     * @param int $inuid    id da tabela instrumentounidade usado na consulta
     */
    public function verificarDirigenteMunicipal($inuid)
    {
        //verifica se o parametro $inuid não é vazio
        if(empty($inuid)){
            return false;
        }

        //consulta para obter os dados do Dirigente Municipal de Educação e o código do município
        $sqlPar = "SELECT DISTINCT ie.entnome, ie.entcpf, iu.muncod
                FROM par3.instrumentounidade_entidade ie
                INNER JOIN par3.instrumentounidade iu ON iu.inuid = ie.inuid
                WHERE ie.tenid = ".self::DIRIGENTE."
                AND ie.entstatus = 'A'
                AND iu.inuid = {$inuid} ";

        $dirigentes = $this->carregar($sqlPar, 0, 30);

        //se for encontrado mais de um registro, ou nenhum registro, considera o cadastro incosistente.
        if(empty($dirigentes) || count($dirigentes) > 1 ){
            return false;
        }

        $sqlSeguranca =     "SELECT ur.usucpf, u.usunome, ur.muncod, ur.estuf
                                FROM seguranca.usuario u
                                INNER JOIN seguranca.perfilusuario p   on u.usucpf = p.usucpf
                                INNER JOIN par3.usuarioresponsabilidade ur on ur.usucpf = u.usucpf and ur.pflcod = p.pflcod
                                WHERE p.pflcod = ".Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL."
                                AND ur.rpustatus = 'A'
                                AND ur.usucpf = '{$dirigentes[0]['entcpf']}'";


        $usuarios = $this->carregar($sqlSeguranca, 0, 30);

        //verifica se houve mais de um, ou nenhum resultado
        if( empty($usuarios) /*|| count($usuarios) > 1*/ ) //OBS rever regra de possuir mais de uma responsabilidade
        {
            return false;
        }
        //se for encontrado apenas um registro e os dados baterem considera que os cadastros estão consistentes
        else if($usuarios[0]['muncod'] == $dirigentes[0]['muncod'])
        {
            return true;
        }
    }//end verificarDirigenteMunicipal

    /**
    * Função para verificar se o usuário já tem o perfil de dirigente municipal associado a
    * outro município para impedir que seja feita a associação a vários municípios
    *
    * @param string    $usucpf     CPF do usuário que será verificado
    * @param string    $muncod     código do município que será tratado como excessão
    *
    * @return boolean      True se o dirigente estiver cadastrado em outro munícpio além do informado
    *                      e false, caso contrário.
    *
    */
   public static function dirigenteOutroMunicipio($usucpf, $muncod)
   {
       global $db;

       $sql = "SELECT ur.usucpf, u.usunome, ur.muncod, ur.estuf, u.usustatus, ur.rpustatus
               FROM seguranca.usuario u
               INNER JOIN seguranca.perfilusuario p   on u.usucpf = p.usucpf
               INNER JOIN par3.usuarioresponsabilidade ur on ur.usucpf = u.usucpf and ur.pflcod = p.pflcod
               WHERE p.pflcod = ".Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL."
               AND rpustatus = 'A'
               AND ur.usucpf = '{$usucpf}'
               AND ur.muncod <> '{$muncod}';";

       $municipios = $db->carregar($sql);

       if(is_array($municipios) && count($municipios) > 0){
           //se algum resultado foi encontrado retorna true
           return true;
       }
       else{
           //se nenhum resultado foi encontrado retorna false
           return false;
       }

   }// end dirigenteOutroMunicipio

   public function getDadosEntidade( $inuid, $tenid )
   {
	   	$sql = "
		   	SELECT
		   		ent.entid,
				ent.inuid,
				ent.tenid,
				ent.endid,
				ent.entcpf,
				ent.entnome,
				ent.entemail,
				ent.enttelefonecomercial,
				ent.enttelefonecelular,
				ent.entcnpj,
				ent.entinscricaoestadual,
				ent.entrazaosocial,
				ent.entsigla,
				ent.entstatus,
				ent.entdtinclusao,
				ent.entrg,
				ent.entorgexpedidor,
				ent.entdtnascimento,
				ent.enttelefonefax,
				ent.entnivelensino,
				ent.entempoatuacao,
				ent.entcursomec,
				ent.entcursomecdescricao,
				ent.entdtinativacao,
				ent.entatuacao,
				ent.entcargo,
				ent.entsexo,
				ent.entusucpfinativacao,
				ent.entdtcriacao,
				ent.entusucpfinclusao,
				ede.endcep,
				ede.endlogradouro,
				ede.endcomplemento,
				ede.endnumero,
				ede.endbairro
		   	FROM
		   	par3.instrumentounidade_entidade ent
		   	INNER JOIN par3.endereco ede ON ede.endid = ent.endid
		   	WHERE
		   	entstatus = 'A'
		   	AND
		   	inuid = {$inuid}
		   	AND
		   	tenid = {$tenid}
		   	ORDER BY entdtinclusao DESC
		   	limit 1
	   	";

	   	$result = $this->pegaLinha($sql);
	   	$result = ($result) ? $result : Array();

	   	return $result;
   }

   public function getDadosEXEntidade( $inuid, $tenid )
   {
	   	$sql = "SELECT
                    *
    		   	FROM
    		   	  par3.instrumentounidade_entidade ent
    		   	WHERE
        		   	entstatus = 'I'
        		   	AND inuid = {$inuid}
        		   	AND tenid = {$tenid}
    		   	ORDER BY entdtinclusao DESC
    		   	LIMIT 1";

	   	$result = $this->pegaLinha($sql);
	   	$result = ($result) ? $result : Array();
	   	return $result;
   }



   /**
    *
    * */
	private function inativaEntidadesAtuais($params){

		$inuid = $params['inuid'];
		$tenid = $params['tenid'];

   		$sql = "
	   		UPDATE
	   			par3.instrumentounidade_entidade
	   		SET
	   			entstatus = 'I',
	   			entdtinativacao = 'NOW()',
	   			entusucpfinativacao = '00000000191'
	   		WHERE
	   			entstatus = 'A'
	   		AND
	   			inuid = {$inuid}
	   		AND
	   			tenid = {$tenid}"
   		;
   		return $this->executar($sql);
   }

   /**
    *
    * */
   public function insereNovoDirigentesEntidadeRotina( $params )
   {
	   	/*** CARREGA VARIÁVEI COM OS PARAMETROS*/
	   	$inuid 				= $params['inuid'];
	   	$tenid 				= $params['tenid'];
	   	$arrNovaEntidade 	= $params['arrNovaEntidade'];
	   	$arrEntidadeAntiga 	= $params['arrEntidadeAntiga'];

	   	/*** VERIFICA A EXISTENCIA DO CPF A SER INSERIDO, POIS É INDISPENSÁVEL PARA A FUNCIONALIDADE*/
	   	if( $arrNovaEntidade['entcpf'] )
	   	{
	   		/*** INATIVA  A ANTIGA */
	   		$params['inuid'] = $inuid;
	   		$params['tenid'] = $tenid;

	   		$retornoInativacao = $this->inativaEntidadesAtuais($params);

	   		if($retornoInativacao)
		   	{
		   		/*** TRATA VARIAVEIS*/
		   		$arrNovaEntidade['entnumdddcomercial'] = trim($arrNovaEntidade['entnumdddcomercial']);
		   		$arrNovaEntidade['entnumcomercial'] = trim($arrNovaEntidade['entnumcomercial']);

		   		$cpf 				= ($arrNovaEntidade['entcpf'])                    ? "'{$arrNovaEntidade['entcpf']}'"                : 'NULL';
		   		$nome 				= ($arrNovaEntidade['entnome'])                   ? "'{$arrNovaEntidade['entnome']}'"               : 'NULL';
		   		$apelido            = explode(' ', $arrNovaEntidade['entnome']);
		   		$apelido            = $apelido[0]                                     ? "'{$apelido[0]}'"                               : 'NULL';
		   		$email				= ($arrNovaEntidade['entemail'])                  ? "'{$arrNovaEntidade['entemail']}'"               : "''";
		   		$telefoneComercial	= ($arrNovaEntidade['entnumcomercial'])      	  ? "'{$arrNovaEntidade['entnumdddcomercial']}{$arrNovaEntidade['entnumcomercial']}'" 	  : 'NULL';
		   		$dddComerc	        = ($arrNovaEntidade['entnumdddcomercial'])        ? "'".preg_replace("/[^0-9]/", "", $arrNovaEntidade['entnumdddcomercial'])."'" 	: 'NULL';
		   		$numComerc	        = ($arrNovaEntidade['entnumcomercial'])           ? "'{$arrNovaEntidade['entnumcomercial']}'" 	    : 'NULL';
		   		$rg 				= ($arrNovaEntidade['entrg'])                     ? "'{$arrNovaEntidade['entrg']}'"                 : 'NULL';
		   		$orgaoExpedidor		= ($arrNovaEntidade['entorgexpedidor'])           ? "'{$arrNovaEntidade['entorgexpedidor']}'"       : 'NULL';
		   		$dtNascimento	 	= ($arrNovaEntidade['entdtnascimento'])           ? "'{$arrNovaEntidade['entdtnascimento']}'"       : 'NULL';
		   		$sexo 				= ($arrNovaEntidade['entsexo'])                   ? "'".strtoupper($arrNovaEntidade['entsexo'])."'" : 'NULL';

		   		$cep 				= ($arrNovaEntidade['endcep'])                    ? "'{$arrNovaEntidade['endcep']}'"                : "''";
		   		$logradouro 		= ($arrNovaEntidade['endlogradouro'])             ? "'{$arrNovaEntidade['endlogradouro']}'"         : "''";
		   		$complemento 		= ($arrNovaEntidade['endcomplemento'])            ? "'{$arrNovaEntidade['endcomplemento']}'"        : "''";
		   		$numeroEnd 			= ($arrNovaEntidade['endnumero'])                 ? "'{$arrNovaEntidade['endnumero']}'"             : "''";
		   		$bairro				= ($arrNovaEntidade['endbairro'])                 ? "'{$arrNovaEntidade['endbairro']}'"             : "''";

		   		$muncod		        = ($arrNovaEntidade['co_municipio_ibge_dirigente'])   ? "'{$arrNovaEntidade['co_municipio_ibge_dirigente']}'" : 'NULL';
		   		$estuf		        = ($arrNovaEntidade['sg_uf_dirigente'])               ? "'{$arrNovaEntidade['sg_uf_dirigente']}'" : 'NULL';

		   		$sqlI =
		   		"INSERT INTO par3.endereco
		   		(
			   		endcep,
			   		endlogradouro,
			   		endcomplemento,
			   		endnumero,
			   		endbairro
		   		)
		   		VALUES
		   		(
			   		{$cep},
			   		{$logradouro},
			   		{$complemento},
			   		{$numeroEnd},
			   		{$bairro}
		   		)

		   		RETURNING endid;
		   		";
		   		$endid = $this->pegaUm($sqlI);


		   		/*** INSERE A NOVA*/
		   		$insert = "
		   		INSERT INTO par3.instrumentounidade_entidade
		   		(
			   		inuid,
			   		tenid,
			   		entcpf,
			   		entnome ,
			   		entemail,
			   		entstatus,
			   		enttelefonecomercial,
			   		entdtinclusao,
			   		entrg,
			   		entorgexpedidor,
			   		entdtnascimento,
			   		entsexo,
			   		entdtcriacao,
			   		entusucpfinclusao,
			   		endid
		   		)
		   		VALUES (
			   		{$inuid},
			   		{$tenid},
			   		{$cpf},
			   		{$nome},
			   		{$email},
			   		'A',
			   		{$telefoneComercial},
			   		'NOW()',
			   		{$rg},
			   		{$orgaoExpedidor},
			   		{$dtNascimento},
			   		{$sexo},
			   		'NOW()',
			   		'00000000191',
			   		{$endid}
		   		)
		   		RETURNING entid
		   		";
		   		$entid = $this->pegaUm($insert);

		   		return ($entid) ? TRUE : FALSE;
		   	}
		   	else
		   	{
		   		return false;
		   	}
		}
		else
		{
			return false;
		}
   }

   /***
    *
    *
    * @param unknown $params
    * @return boolean*/
   public function attDadosDirigentesEntidadeRotina( $params )
   {
	   	/*** CARREGA VARIÁVEI COM OS PARAMETROS*/
	   	$inuid 				= $params['inuid'];
	   	$tenid 				= $params['tenid'];
	   	$arrNovaEntidade 	= $params['arrNovaEntidade'];
	   	$arrEntidadeAntiga 	= $params['arrEntidadeAntiga'];

	   	/*** ENTID */
	   	$entid = $arrEntidadeAntiga['entid'];

	   	/*** VERIFICA A EXISTENCIA DO CPF A SER INSERIDO, POIS É INDISPENSÁVEL PARA A FUNCIONALIDADE*/
	   	if( $arrNovaEntidade['entcpf'] )
	   	{
	   		/**Trata os Telefones */
	   		$arrNovaEntidade['entnumdddcomercial'] = trim($arrNovaEntidade['entnumdddcomercial']);
	   		$arrNovaEntidade['entnumcomercial'] = trim($arrNovaEntidade['entnumcomercial']);
	   		/** Entidade */
	   		$arrCampos['entnome']                =  $arrNovaEntidade['entnome'];
	   		$arrCampos['entsexo']                =  $arrNovaEntidade['entsexo'];
	   		$arrCampos['entemail']               =  $arrNovaEntidade['entemail'];
	   		$arrCampos['entrg']                  =  $arrNovaEntidade['entrg'];
	   		$arrCampos['entorgexpedidor']        =  $arrNovaEntidade['entorgexpedidor'];
	   		$arrCampos['entdtnascimento']        =  $arrNovaEntidade['entdtnascimento'];
	   		$arrCampos['enttelefonecomercial']   =  $arrNovaEntidade['entnumdddcomercial'].$arrNovaEntidade['entnumcomercial'];
	   		$arrCampos['enttelefonecelular']     =  $arrNovaEntidade['enttelefonecelular'];

	   		/** Endereco */
	   		$arrCamposEnd['endcep']                 =  $arrNovaEntidade['endcep'];
	   		$arrCamposEnd['endlogradouro']          =  $arrNovaEntidade['endlogradouro'];
	   		$arrCamposEnd['endcomplemento']         =  $arrNovaEntidade['endcomplemento'];
	   		$arrCamposEnd['endnumero']              =  $arrNovaEntidade['endnumero'];
	   		$arrCamposEnd['endbairro']              =  $arrNovaEntidade['endbairro'];

	   		/** MONTA AS COLUNAS DA ENTIDADE*/
	   		$arrColunas = array();
	   		foreach($arrCampos as $nome => $valor)
	   		{
	   			if( ($valor != '') && ($valor != NULL) && ($valor != '0' ))
	   			{
	   				$arrColunas[] =  " {$nome} = '{$valor}' ";
	   			}
	   		}
	   		/** MONTA AS COLUNAS DO ENDEREÇO DA ENTIDADE*/
	   		foreach($arrCamposEnd as $nome => $valor)
	   		{
	   			if( ($valor != '') && ($valor != NULL) && ($valor != '0' ))
	   			{
	   				$arrColunasEnd[] =  " {$nome} = '{$valor}' ";
	   			}
	   		}
	   		/**CARREGA OS IDS*/
	   		$endid = $arrEntidadeAntiga['endid'];

	   		/**ATUALIZA OS DADOS DA ENTIDADE*/
	   		if( (count($arrColunas) > 0) && ($entid) )
	   		{
	   			$colunas = implode($arrColunas, ',');
	   			$sqlUpdate = " UPDATE par3.instrumentounidade_entidade SET {$colunas}  WHERE entid = {$entid} ";
	   			$this->executar($sqlUpdate);
	   		}

	   		/**ATUALIZA OS DADOS DO ENDEREÇO DA ENTIDADE*/
	   		if( (count($arrColunasEnd) > 0) && ($endid) )
	   		{
	   			$colunasE = implode($arrColunasEnd, ',');
	   			$sqlUpdate2 = " UPDATE par3.endereco SET {$colunasE}  WHERE endid = {$endid} ";
	   			$this->executar($sqlUpdate2);
	   		}
	   	}
	   	if( $entid )
	   	{
	   		return true;
	   	}
	   	else
	   	{
	   		return false;
	   	}
   }

    /**
     * Calcular a quantidade de registros de par3.instrumentounidade_entidade
     * para determinado instrumento_unidade
     * @param $array['tenid','inuid']
     * @return integer.
     */
    public function calcularUnidadeEntidades($arrPost)
    {
        $sql                = $this->montarSQLSimplesEntidades($arrPost);
        $rsUnidadeEntidades = $this->carregar($sql);
        if(!$rsUnidadeEntidades){
            return 0;
        }
        return count($rsUnidadeEntidades);
    }

    public function recuperarNutricionistasPendentes()
    {
        $sql = <<<SQL
SELECT int.entid as id,entcpf, entnome, entemail,enttelefonecomercial,tendsc, sndescricao,danid,usu.usuemail
FROM par3.instrumentounidade_entidade  int
INNER JOIN par3.tipoentidade             ten ON ten.tenid  = int.tenid
INNER JOIN seguranca.usuario             usu ON int.entcpf = usu.usucpf
INNER JOIN par3.instrumentounidade       ine ON int.inuid  = ine.inuid
INNER JOIN par3.vinculacaonutricionista  vn  ON vn.tenid   = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND vn.vnstatus = 'A'
INNER JOIN par3.situacaonutricionista    sn  ON sn.snid    = vn.snid
left join  par3.dadosnutricionista       dan ON dan.dancpf = vn.vncpf
WHERE
   int.tenid IN (8, 7,16)
   AND int.entstatus = 'A'
   AND sn.snid = 3
   ORDER BY tendsc desc
   
SQL;
        return $this->carregar($sql);
    }

    public function pegarNomeEntidade($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $entidade = $modelInstrumentoUnidade->inudescricao;
        $estuf = $modelInstrumentoUnidade->estuf;
        $itrid = $modelInstrumentoUnidade->itrid;

        return ($itrid == 1) ? $entidade : $entidade . ' - ' . $estuf;
    }

    public function enviarEmailNutricionista($dados, $novo)
    {
        $itrDsc = $this->pegarNomeEntidade($dados['inuid']);

        if($dados['itrid' == '2']) {
            $municipio = $itrDsc;
            $tipoEsfera = 'Município';
            $unidade = $itrDsc;
            $strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar  " . $itrDsc ;
        } else {
            $tipoEsfera = 'Estado';
            $unidade = $itrDsc;
            $strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar  " . $itrDsc ;
        }

        if ($novo) {
            $strMensagem =
                "<pre align=\"center\" style=\"text-align: justify;\"  >
Você foi cadastrado como Nutricionista no âmbito do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que
você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".
 
http://simec.mec.gov.br
Usuário: {$dados['usucpf']}
Senha: {$dados['ususenha']}
 
Em caso de dúvidas, entrar em contato pelo PAR Fale Conosco, no endereço www.fnde.gov.br/parfaleconosco/index.php/publico ou no sítio do FNDE em Programas>PAR>Contatos>Acesso para usuário público.
 
 
Atenciosamente,
Coordenação de Segurança Alimentar e Nutricional - SIMEC/PAR
</pre>";
        } else {
            $strMensagem = "<pre align=\"center\" style=\"text-align: justify;\"  >
Você foi cadastrado como Nutricionista no âmbito do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha atuais de cadastro no SIMEC e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que
você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".
 
http://simec.mec.gov.br
 
 
Caso não lembre a sua senha acesse o link http://simec.mec.gov.br/recupera_senha.php
Se mesmo assim não conseguir acessar o sistema, entrar em contato pelo PAR Fale Conosco, no endereço www.fnde.gov.br/parfaleconosco/index.php/publico ou no sítio do FNDE em Programas>PAR>Contatos>Acesso para usuário público.
Equipe Técnica do PAR - Plano de Ações Articuladas
 
 
 
Atenciosamente,
Coordenação de Segurança Alimentar e Nutricional - SIMEC/PAR
</pre>";
        }

        $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
        $strMensagem = html_entity_decode($strMensagem);

        if(!empty($dados['entemail'])) {
            $strEmailTo = $dados['entemail'];
        }else{
            $strEmailTo = $dados['usuemail'];
        }

        if($_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br")
        {

//            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);

        }
        else if($_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == 'simec-dsv')
        {
//            ver($strMensagem);
        }
        else if($_SERVER['HTTP_HOST'] == "simec.mec.gov.br")
        {

            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }
    }

    public function getEntPorCpf($cpf)
    {
        $dados = $this->pegaLinha("select * from par3.instrumentounidade_entidade where entcpf = '{$cpf}' and entstatus = 'A'");
        return $dados;
    }

    public function resetarSenhaNutricionistasPorCPF($usucpf)
    {
        $usuario = new Seguranca_Model_Usuario($usucpf);
        $mDan    = new Par3_Model_DadosNutricionista();
        $mDan->carregarDadosPorCPF($usucpf);
        $mEnt    = $this->getEntPorCpf($usucpf);
        if($usuario->usucpf && $mEnt['entcpf']) {
            $senhageral = 'nut' . substr($usucpf, -3);
            $senha = md5_encrypt_senha($senhageral, '');
            $dadosUsuario['usucpf']   = $usuario->usucpf;
            $dadosUsuario['usunome']  = $usuario->usunome;
            $dadosUsuario['usuemail'] = $usuario->usuemail;
            $dadosUsuario['ususenha'] = $senha;
            $dadosUsuario['usuchaveativacao'] = 'F';
            $dadosUsuario['suscod'] = 'A';

            $usuario->popularDadosObjeto($dadosUsuario);
            $usuario->salvar();
            $usuario->commit();

            $usuarioSistema = new Seguranca_Model_Usuariosistema();
            $usuSis = $usuarioSistema->recuperarTodos('*',array( "usucpf = '{$usuario->usucpf}' AND pflcod = ".PAR_PERFIL_NUTRICIONISTA." AND sisid = ".SISID_PAR));
            if(empty($usuSis[0])) {
                //Adiciona o perfil de nutricionista para acesso ao par antigo
                $arrayUsuarioSistema = [];
                $arrayUsuarioSistema['usucpf'] = $usuario->usucpf;
                $arrayUsuarioSistema['sisid']  = SISID_PAR;
                $arrayUsuarioSistema['suscod'] = 'B';
                $arrayUsuarioSistema['pflcod'] = PAR_PERFIL_NUTRICIONISTA;
                $usuarioSistema->salvar($arrayUsuarioSistema);
                $usuarioSistema->commit();
            }

            $perfil_usuario = new Seguranca_Model_Perfilusuario();
            $usuPar = $perfil_usuario->recuperarTodos('*',array( "usucpf = '{$usuario->usucpf}' AND pflcod = ".PAR_PERFIL_NUTRICIONISTA));

            if(empty($usuPar[0]))
            {
                $dadosPerfil['usucpf'] = $usuario->usucpf;
                $dadosPerfil['sisid']  = SISID_PAR;
                $dadosPerfil['pflcod'] =  PAR_PERFIL_NUTRICIONISTA;
                $perfil_usuario->salvarDados($dadosPerfil);
            }

            $usuPar = $perfil_usuario->recuperarTodos('*',array( "usucpf = '{$usuario->usucpf}' AND pflcod = ".Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA));

            if(empty($usuPar[0])){
                $dadosPerfil['usucpf'] = $usuario->usucpf;
                $dadosPerfil['sisid']  = PAR3_SIS_ID;
                $dadosPerfil['pflcod'] =  Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA;
                $perfil_usuario->salvarDados($dadosPerfil);
            }

            $dadosUsuario['ususenha'] = $senhageral;
            $emails = [
                $mDan->danemailprincipal,
                $mDan->danemailalternativo,
                $mEnt['entemail'],
                $usuario->usuemail
            ];
            $emails = array_filter($emails);
            $dadosUsuario['usuemail'] = array_unique($emails,SORT_STRING);
            $dadosUsuario['inuid'] = $mEnt['inuid'];
            $this->enviarEmailNutricionista($dadosUsuario, 1);
            return true;
        }
        return false;
    }
}//end Class

?>
