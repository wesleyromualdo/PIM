<?php

/**
 * Classe Metas
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 *
 * Objeto de Modelo de Metas
 * TODO classe não testada
 */
class Metas extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.meta";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('metid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'metid' => null,
		'mettitulo' => null,
		'metfontemunicipio' => null,
		'metfonteestado' => null,
		'metstatus' => null,
		'metchamada' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'mettitulo'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'metid',
		'mettitulo',
		'metfontemunicipio',
		'metfonteestado',
		'metstatus'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'mettitulo',
		'metfontemunicipio',
		'metfonteestado',
		'metstatus'
	);

	/**
	 * RetornaSQLListaGeralMetas
	 *
	 * @param array $filtros
	 * @author Eduardo Dunice
	 * @return $sql
	 */
	public function retornaSQLListaGeralMetas()
	{
		$sql = "SELECT * FROM sase.meta WHERE metstatus = 'A' ORDER BY metid";

		return $sql;
	}

	/**
	 * RetornaArrayListaGeral
	 *
	 * @param array $filtros
	 * @author Eduardo Dunice
	 * @return array $arrMetas
	 */
	public function retornaArrayListaGeral()
	{
		$sql = self::retornaSQLListaGeralMetas();

		return self::carregar($sql);
	}

	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT
				'<a style=\"cursor:pointer\" onclick=\"editarMetas( ' || metid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirMetas( ' || metid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				mettitulo,
				CASE WHEN metstatus = 'I' THEN 'Inativo' WHEN metstatus = 'A' THEN 'Ativo' END AS metstatus
			FROM " . $this->stNomeTabela . " ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}

		return $sql;
	}

	/**
	 * Atualiza Metas
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarMetas(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente o Metas
 	 *
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirMetas( $metid ){
		$this->carregarPorId( $metid );
		$this->arAtributos['metstatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaMetas( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Metas Montado no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarMetas(){
		if( $this->validaCamposObrigatorios() ){
			$this->metid = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}


	/**
	 * Retornar Array de Submetas dessa meta
	 *
	 * @author Eduardo Dunice
	 * @return bool|array
	 */
	public function retornarArraySubmetas($metid = null)
	{
	    $metid = $metid ? $metid : $this->metid;

		$sql = self::retornarSQLSubmetas($metid);

		$arrSubmetas = self::carregar($sql);

		if ($arrSubmetas[0]['subid'] != '') return $arrSubmetas;

		return false;
	}//end retornarArraySubmetas();


	/**
	 * Retornar SQL de Submetas dessa meta
	 *
	 * @author Eduardo Dunice
	 * @return string
	 */
	public function retornarSQLSubmetas($metid)
	{
		$sql = "SELECT *
                FROM sase.submeta
		        WHERE metid = $metid";

		return $sql;
	}//end retornarArraySubmetas();

    /**
     *
     * @author Victor Martins Machado
     * @param string $tipo - Objetivo do SQL:
     *      p - Retorna uma lista com os indicadores pendentes
     *      n - Retorna o número de indicadores
     * $return string - SQL tratado de acordo com o tipo
     */
    public function retornaSQLIndicadores($estuf, $muncod, $itrid, $tipoSql = 'p')
    {
        $where = $sql = $metids = $tipo = $colunas = $group = '';
        $ands = array();
        if($itrid === '2'){
            $metids = "13,14,17,19,20";
            $tipo = 'M';
            $where = "muncod = '{$muncod}'";
        } else {
            $metids = "19,20";
            $tipo = 'E';
            $where = "estuf = '{$estuf}'";
        }

        switch($tipoSql){
            case 'p':
                $colunas = "
                    subiddial,
                    subtitulo,
                    metid
                    ";
                $metidCol = "sub.metid,";
                $ands[] = "AND (ppd.ppdid IS NULL OR (ppd.pnesemvalor = 'f' AND ppd.pnevalormeta IS NULL))";
                $ands[] = "AND mic.micid IS NULL";
                $ands[] = "WHERE (sub.ppdid IS NULL OR (sub.pnepossuiplanoremvigente IS NULL AND sub.pneplanorefcaput IS NULL))";
                $group = "ORDER BY metid";
                break;

            case 'n':
                $colunas = " count(subiddial) ";
                $metidCol = "";
                $ands[] = "";
                $ands[] = "";
                $ands[] = "";
                $group = "";
                break;
        }

        $sql = <<<DML
            SELECT
                {$colunas}
            FROM (
                SELECT
                    {$metidCol}
                    sub.subiddial,
                    subtitulo
                FROM sase.submetadial sub
            	INNER JOIN sase.pneanoexibicaodial 	pae ON pae.subiddial  = sub.subiddial AND pae.paetipo = '{$tipo}' AND pae.paestatus = 'A'
                LEFT  JOIN sase.pneprevdial 		ppd ON ppd.subid      = sub.subiddial AND ppd.pnedatainclusao = (SELECT max(pnedatainclusao) FROM sase.pneprevdial WHERE subid = sub.subiddial)
                WHERE
                    sub.substatus = 'A'
                    AND sub.metid NOT IN ($metids)
                    AND sub.metid != 18
                    {$ands[0]}
                    AND ppd.pne{$where}
            UNION
                SELECT
                    {$metidCol}
                    sub.subiddial,
                    subtitulo
                FROM sase.submetadial sub
                LEFT JOIN sase.pneanoexibicaodial   pae ON pae.subiddial = sub.subiddial AND pae.paetipo = '{$tipo}' AND pae.paestatus = 'A'
                LEFT JOIN sase.metainfcomplementar  mic ON mic.metid = sub.metid and mic.{$where}
                WHERE
                    sub.metid IN ($metids)
                    {$ands[1]}
                ) as sub
            {$group}
DML;
// ver($sql);
        return $sql;
    }
	/**
	 * retornarNumeroMetasPreenchidas
	 *
	 * @author Pedro Dantas
	 * @return array
	 */
	public function retornarNumeroIndicadores($estuf = null, $muncod = null, $itrid = null)
	{
        $sql = $this->retornaSQLIndicadores($estuf, $muncod, $itrid, 'n');
		return $this->pegaUm($sql);
	}

	/**
	 * retornarNumeroMetasPreenchidas
	 *
	 * @author Pedro Dantas
	 * @return array
	 */
	public function retornarIndicadoresPendentes($estuf = null, $muncod = null, $itrid = null)
	{
        $sql = $this->retornaSQLIndicadores($estuf, $muncod, $itrid);
		return $this->carregar($sql);
	}

}//end Class
?>