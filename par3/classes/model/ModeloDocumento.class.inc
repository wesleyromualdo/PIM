<?php
class Par3_Model_Modelo_Documento extends Modelo
{
	/*
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "par3.modelodocumento";

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('mdoid');

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
    protected $arAtributosTabela     = array('mdoid', 'tpdcod','mdonome', 'mdoconteudo', 'mdostatus', 'usucpf', 'mdodatainclusao', 'mdovalidacaogestor', 'mdovalidacaofnde', 'mdoqtdvalidacao',
									  	'mdotipovinculado', 'mdovisivel', 'iutid', 'mdoex_oficio', 'mdoesfera', 'mdotipo');
    
    protected $arAtributos     = array( 
									  	'tpdcod' 				=> null,
    									'mdonome' 				=> null, 
									  	'mdoconteudo' 			=> null, 
									  	'mdostatus' 			=> null,
									  	'usucpf' 				=> null,
									  	'mdodatainclusao' 		=> null,
									  	'mdovalidacaogestor' 	=> null,
									  	'mdovalidacaofnde'		=> null,
									  	'mdoqtdvalidacao' 		=> null,
									  	'mdotipovinculado' 		=> null,
									  	'mdovisivel' 			=> null,
									  	'iutid' 				=> null,
									  	'mdoex_oficio'			=> null,
									  	'mdoesfera'				=> null,
									  	'mdotipo'				=> null
									  );
								  
	function __construct() {
		parent::__construct();
	}
	
	public function recuperar($arrPost)
	{
		return $this->model;
	}
	
	public function salvar_modelo( $arrPost = array() ){
		ver($arrPost,d);
		$sql = "INSERT INTO par3.modelodocumento(tpdcod, mdonome, mdoconteudo, mdostatus, usucpf, mdodatainclusao, 
            mdovalidacaogestor, mdovalidacaofnde, mdoqtdvalidacao, mdotipovinculado, 
            mdovisivel, iutid, mdotipo)
    VALUES (?, ?, ?, ?, ?, ?, ?, 
            ?, ?, ?, ?, 
            ?, ?, ?)";
	}
	public function listar( $arrPost = array() ){
		
		$filtro = '';
		if( $arrPost['mdonome'] ) $filtro .= " and em.mdonome ilike '%{$arrPost['mdonome']}%' ";
		if( $arrPost['tpdcod'] ) $filtro .= " and tp.tpdcod = {$arrPost['tpdcod']} ";
		if( $arrPost['mdoesfera'] ) $filtro .= " and em.mdoesfera = '{$arrPost['mdoesfera']}' ";
		if( $arrPost['mdotipo'] ) $filtro .= " and em.mdotipo = '{$arrPost['mdotipo']}' ";
		
		$sql = "SELECT
					em.mdoid,
					em.mdoid as codigo, 
                    em.mdonome,
					tp.tpddsc as tipo_documento,
					case when em.mdoex_oficio = 'N' then 'Não' else 'Sim' end as ex_oficio,
					case when em.mdotipo = 'P' then 'PAR' else 'OBRA' end as tipo,
                    case when em.mdoesfera = 'E' then 'Estadual' else 'Municipal' end as esfera,
					case when em.mdovalidacaogestor = 'N' then 'Não' else 'Sim' end as validacao_gestor,
                    case when em.mdovalidacaofnde = 'N' then 'Não' else 'Sim' end as validacao_fnde,
                    to_char(em.mdodatainclusao,'DD/MM/YYYY') as data,
					su.usunome
                                        
				FROM
					par3.modelodocumento em
					INNER JOIN public.tipodocumento tp ON tp.tpdcod = em.tpdcod
					INNER JOIN seguranca.usuario su ON su.usucpf = em.usucpf
				WHERE
					em.mdostatus = 'A'
					and em.mdovisivel = 'S'
					$filtro
				ORDER BY
					em.mdoid";
		return $sql;
	}
}