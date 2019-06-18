<?php

/**
 * Created by PhpStorm.
 * User: danielfiuza
 * Date: 20/02/17
 * Time: 14:32
 */
class Par3_Model_ConfiguracaoParUnidade extends Modelo
{
    protected $stNomeTabela = 'par3.configuracaopar_unidade';

    protected $arChavePrimaria = array('couid',);

    protected $arChaveEstrangeira = array('usucpf', 'inuid', 'tcpid');

    protected $arAtributos = array(
        'couid' => null,
        'usucpf' => null,
        'inuid' => null,
        'tcpid' => null,
        'couvalor' => null,
        'coutexto' => null,
        'coustatus' => null,
        'coudtinclusao' => null,
    );

    /**
     * Função que muda o status para inativo de todos os
     * registros da tabela do mesmo tipo
     * @param $tcpid
     * @return $sqlUpdate
     */
    public function updateStatusConfUniPar($tcpid, $inuid)
    {
        $sqlUpdate = "UPDATE par3.configuracaopar_unidade SET coustatus = 'I' WHERE tcpid = $tcpid AND inuid = $inuid";
        return $sqlUpdate;
    }

    public function retornaConfiguracaoUnidadeSQL($inuid,$tcpid)
    {
        $sql = "SELECT 
	                coalesce(couvalor, copvalor) as qtd
                FROM 
                    par3.configuracaopar_unidade cou
                LEFT JOIN par3.configuracoespar cop ON cop.tcpid = cou.tcpid AND cop.copstatus = 'A'
                WHERE
                    cou.inuid = {$inuid}
                    AND cou.tcpid = {$tcpid}
                    AND cou.coustatus = 'A' ";
        return $sql;
    }
}