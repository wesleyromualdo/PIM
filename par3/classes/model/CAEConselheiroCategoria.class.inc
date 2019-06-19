<?php
/**
 * Classe de controle Conselheiro Categoria
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */

/**
 * Par3_Model_CAEConselheiroCategoria
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */
class Par3_Model_CAEConselheiroCategoria extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.cae_conselheiro_categoria';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'cctid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cctid' => null,
        'cctdsc' => null,
    );


    /**
     * Função pegarSQLSelectCombo
     * - monta o sql para o combo de cargo da equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function pegarSQLSelectCombo($arrPost = array())
    {
        $sql = "SELECT cctid as codigo, cctdsc as descricao FROM par3.cae_conselheiro_categoria ORDER BY descricao";

        return $sql;
    }//end formPessoaJuridica()


    /**
     * Função carregarJSONCombo
     * - monta JSON para combo
     *
     * @return string [JSON].
     *
     */
    public function carregarJSONCombo($arrPost = array())
    {
        $sql = self::pegarSQLSelectCombo($arrPost);

        $arrDados = self::carregar($sql);

        return simec_json_encode($arrDados);
    }//end carregarJSONCombo()


    /**
     * Função pegarSQLSelectCombo
     * - monta o sql para o combo de cargo da equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function pegarSQLSelectComboSubCategoria($arrPost = array())
    {
    	$sql = "SELECT cscid as codigo, cscdsc as descricao
    			FROM par3.cae_conselheiro_subcategoria
    			WHERE cctid = {$arrPost['cctid']}
    			ORDER BY descricao";

    	return $sql;
    }//end formPessoaJuridica()


    /**
     * Função carregarJSONCombo
     * - monta JSON para combo
     *
     * @return string [JSON].
     *
     */
    public function carregarJSONComboSubCategoria($arrPost = array())
    {
    	$sql = self::pegarSQLSelectComboSubCategoria($arrPost);

    	if($arrPost['cctid'] != '') $arrDados = self::carregar($sql);
    	$arrDados = is_array($arrDados) ? $arrDados : array();

    	return simec_json_encode($arrDados);
    }//end carregarJSONCombo()

}//end Class
?>