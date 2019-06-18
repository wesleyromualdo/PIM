<?php

/**
 * Classe de controle das Configurações PAR3
 *
 * @category Class/Render
 * @package  A1
 * @author   Daniel Fiuza <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17/02/2017
 * @link     no link
 */
class Par3_Model_ConfiguracoesPar extends Modelo
{
    protected $stNomeTabela = 'par3.configuracoespar';

    protected $arChavePrimaria = array('copid',);

    protected $arChaveEstrangeira = array('usucpf','tcpid');

    protected $arAtributos = array(
        'copid'        => null,
        'usucpf'       => null,
        'tcpid'        => null,
        'copvalor'     => null,
        'copstatus'    => null,
        'copdtinclusao' => null
    );

    /**
     * Função que muda o status para inativo de todos os
     * registros da tabela do mesmo tipo
     * @param $tcpid
     * @return $sqlUpdate
     */
    public function updateStatusConfPar($tcpid)
    {
        $sqlUpdate = "UPDATE par3.configuracoespar SET copstatus = 'I' WHERE tcpid = $tcpid";
        return $sqlUpdate;
    }

    public function montarFiltroLista($arrPost)
    {
        $where = array('1=1');
        if ($arrPost['copstatus'] != '') $where[] = "cop.copstatus = '{$arrPost['copstatus']}'";
        if ($arrPost['tcpid'] != '') $where[] = "cop.tcpid = '{$arrPost['tcpid']}'";
        return $where;
    }

    public function retornaConfiguracaoGlobal($tcpid)
    {
        $sql = "SELECT * FROM $this->stNomeTabela WHERE tcpid = $tcpid AND copstatus= 'A' ";
        $res =  $this->pegaLinha($sql, 0, 30);
                $this->commit();
        return $res;
    }

    public function pegarSQLlista($arrPost = null)
    {
        $whereSql = "";
        if(!empty($arrPost)){
            $where = self::montarFiltroLista($arrPost);
             $whereSql = "WHERE ".implode(' AND ', $where);
        }

        $sql = "SELECT
                        cop.copid as id,
                    	tcp.tcpdsc as descricao,
                    	cop.copvalor,
                    	usu.usunome,
                    	cop.copdtinclusao,
                    	cop.copstatus
                    FROM par3.configuracoespar cop
                    INNER JOIN seguranca.usuario          usu ON usu.usucpf  = cop.usucpf
                    INNER JOIN par3.tipoconfiguracoespar  tcp ON tcp.tcpid = cop.tcpid
                    ".($whereSql ? $whereSql : ' ')." ORDER BY copstatus,descricao,copdtinclusao DESC";

        return $sql;
    }//end formPessoaJuridica()
}