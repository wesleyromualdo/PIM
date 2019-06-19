<?php

/**
 * Classe Orgao
 *
 * @author SÃ¡vio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de Orgao
 */
class TipoOrgao extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.tipoorgao";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('torid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'torid' => null,
		'infterid' => null,
		'entid' => null,
		'tordesc' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'tordesc'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'torid',
		'infterid',
		'entid',
		'tordesc'
	);

	/**
	 * Campos ObrigatÃ³rios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'infterid',
		'entid',
		'tordesc'
	);

      function salvarTipoOrganizacao(){

        try{


            $sql ="INSERT INTO sase.tipoorgao (
                                  infterid,
                                  entid,
                                  tordesc)
                                VALUES (
                                   {$_POST[infterid]},
                                  '{$_POST[entid]}',
                                  '{$_POST[tordesc]}')";
            $this->executar($sql);
            $this->commit();

        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }

        return true;
        
    }

    function editarTipoOrganizacao(){
                try{
            
            $sql ="
                    UPDATE sase.tipoorgao
                    SET 
                     infterid = '{$_POST[infterid]}',
                     entid = '{$_POST[entid]}',
                     tordesc = '{$_POST[tordesc]}'
                    WHERE torid =   {$_POST[torid]}";
            $this->executar($sql);
            $this->commit();

        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }


        return true;
    }



}
