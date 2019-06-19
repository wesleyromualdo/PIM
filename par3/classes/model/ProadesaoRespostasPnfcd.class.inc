<?php

class Par3_Model_ProadesaoRespostasPnfcd extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.proadesao_respostas_pnfcd";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pppid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'pppid' => null,
									  	'pppresp0' => null,
									  	'pppresp1' => null,
									  	'pppresp1arqid' => null,
									  	'pppresp2' => null,
    									'pppresp2outros' => null,
                                        'pppresp3' => null,
                                        'inuid' => null,
                                        'pppresp3outros' => null,
                                        'docid' => null
									  );

	protected $stOrdem = null;

    public function pegarRespostasPorInuid($inuid)
    {

        $sql = "SELECT
				*
				FROM par3.proadesao_respostas_pnfcd

                WHERE  inuid = {$inuid}";



        return $this->pegaLinha($sql);
    }

}