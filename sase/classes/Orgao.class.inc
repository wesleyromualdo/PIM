<?php

/**
 * Classe Orgao
 *
 * @author SÃ¡vio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de Orgao
 */
class Orgao extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.orgao";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('orgid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'orgid' => null,
		'orgcod' => null,
		'orgdsc' => null,
		'orgstatus' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'orgdsc'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'orgid',
		'orgcod',
		'orgdsc',
		'orgstatus'
	);

	/**
	 * Campos ObrigatÃ³rios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'orgdsc',
		'orgcod',
		'orgstatus'
	);

	/**
	 * Monta query para a funÃ§Ã£o monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author SÃ¡vio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT 
				'<a style=\"cursor:pointer\" onclick=\"editarOrgao( ' || orgid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirOrgao( ' || orgid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				--orgcod,
				orgdsc,
				CASE WHEN orgstatus = 'I' THEN 'Inativo' WHEN orgstatus = 'A' THEN 'Ativo' END AS orgstatus
			FROM " . $this->stNomeTabela . " ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}
		$sql .= "       ORDER BY orgid ASC ";

		return $sql;
	}

	/**
	 * Atualiza Orgao
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author SÃ¡vio Resende
	 */
	public function atualizarOrgao(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente o Orgao
 	 * 
	 * @param integer $stacod
	 * @return bool
	 * @author SÃ¡vio Resende
	 */
	public function excluirOrgao( $orgid ){
		$this->carregarPorId( $orgid );
		$this->arAtributos['orgstatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author SÃ¡vio Resende
	 */
	public function populaOrgao( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author SÃ¡vio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Orgao Montado no Objeto
	 *
	 * @author SÃ¡vio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarOrgao(){
		if( $this->validaCamposObrigatorios() ){
			$this->orgid = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}

}
