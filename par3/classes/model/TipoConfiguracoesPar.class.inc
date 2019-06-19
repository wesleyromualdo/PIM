<?php

/**
 * Classe Par3_Model_TipoConfiguracoesPar
 *
 * @category Class
 * @package  A1
 * @author   Daniel da Rocha Fiuza <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16/02/2017
 * @link     no link
 */
class Par3_Model_TipoConfiguracoesPar extends Modelo
{
    protected $stNomeTabela = 'par3.tipoconfiguracoespar';

    protected $arChavePrimaria = array('tcpid',);

    protected $arChaveEstrangeira = array();

    protected $arAtributos = array('tcpid' => null, 'tcpdsc' => null,);

    const LIMITE_EQUIPE_MUNICIPAL = 1;
    const LIMITE_EQUIPE_ESTADUAL  = 2;

    public function salvarTipoConfiguracao_instrumentoSQL($itrid, $tcpid)
    {
        $sql = "INSERT INTO par3.tipoconfiguracoespar_instrumento(itrid,tcpid) VALUES($itrid,$tcpid)";
        return $sql;
    }

    public function retornaConfiguracaoUnidadeLimite($inuid, $tcpid)
    {
        $mConfiguracaoUnidade = new Par3_Model_ConfiguracaoParUnidade();
        $sql = $mConfiguracaoUnidade->retornaConfiguracaoUnidadeSQL($inuid, $tcpid);
        $objConfiguracaoUnidade = $mConfiguracaoUnidade->pegaUm($sql);
        return $objConfiguracaoUnidade;
    }

    public function pegarSQLSelectCombo()
    {
            $sql = "SELECT
                    	tcpid as codigo,
                    	tcpdsc as descricao
                    FROM
                    	par3.tipoconfiguracoespar
            		ORDER BY descricao";

        return $sql;
    }//end formPessoaJuridica()
}