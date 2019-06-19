<?php
/**
 * Classe de mapeamento da entidade sac.aviso
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */



/**
 * Sac_ModelAviso
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */
class Sac_Model_Aviso extends Modelo
{
    const K_STATUS_ATIVO        = 'A';
    const K_STATUS_INATIVO      = 'I';
    const K_STATUS_FINALIZADO   = 'F';

    const K_EXIBIR_MODAL   = 'M';
    const K_EXIBIR_TARJA   = 'T';
    const K_EXIBIR_AVISO   = 'A';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.aviso';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'aviid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'sisid' => array('tabela' => 'sistema', 'pk' => 'sisid'),
        'usucpfinclusao' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aviid' => null,
        'avidsc' => null,
        'avititulo' => null,
        'avistatus' => null,
        'usucpfinclusao' => null,
        'avidatainclusao' => null,
        'sisid' => null,
        'avidatainicio' => null,
        'avidatafim' => null,
        'avitipoexibicao' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'aviid' => array(  'Digits'  ),
            'avidsc' => array(  ),
            'avititulo' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
            'avistatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'usucpfinclusao' => array(  new Zend_Validate_StringLength(array('max' => 11))  ),
            'avidatainclusao' => array(  ),
            'sisid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)

    public function existePorSistema($sisid)
    {
        $sql = "select count(*)
                from sac.aviso
                where sisid = {$sisid}
                and avistatus = '" . self::K_STATUS_ATIVO . "'
                and now() between coalesce(avidatainicio, now()) and coalesce(avidatafim, now())
                ";
        return $this->pegaUm($sql);
    }

    public function recuperarAvisos($sisid, $usucpf)
    {
        $sql = "select a.aviid, ad.avdid, avidsc, a.avititulo, avitipoexibicao
                from sac.aviso a
                        inner join sac.avisodestinatario ad on ad.aviid = a.aviid
                where sisid = {$sisid}
                and avistatus = '" . self::K_STATUS_ATIVO . "'
                and ad.avdstatus = '" . Sac_Model_AvisoDestinatario::K_STATUS_NAO_LIDO . "'
                and now() between coalesce(avidatainicio, now()) and coalesce(avidatafim, now())
                and usucpf = '$usucpf'
                ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }

    public function listar($sisid, $filtro = array())
    {
        $where[] = "a.sisid = $sisid";
        $where[] = "a.avistatus != '" . self::K_STATUS_INATIVO . "'";
        $join = array();

        if(!empty($filtro['aviid'])){
            $where[] = " a.aviid = '{$filtro['aviid']}' ";
        }
        if(!empty($filtro['pflcod'])){
            $pflcod = (array) $filtro['pflcod'];
            $where[] = " af.pflcod in ('" . implode("', '", $pflcod) . "') ";
            $join[] = ' inner join sac.avisofiltro af on af.aviid = a.aviid ';
        }
        if(!empty($filtro['usuario'])){
            $where[] = " u.usunome ilike '%{$filtro['usuario']}%' ";
            $join[] = ' inner join sac.avisodestinatario ad on ad.aviid = a.aviid
                       inner join seguranca.usuario u on u.usucpf = ad.usucpf ';
        }
        if(!empty($filtro['texto'])){
            $where[] = " ( a.avidsc ilike '%{$filtro['texto']}%' or
                           a.avititulo ilike '%{$filtro['texto']}%'
                         )";
            $join[] = ' inner join sac.avisodestinatario ad on ad.aviid = a.aviid
                       inner join seguranca.usuario u on u.usucpf = ad.usucpf ';
        }

        if(!empty($filtro['muncod'])){
            $muncod = (array) $filtro['muncod'];
            $where[] = " af.muncod in ('" . implode("', '", $muncod) . "') ";
            $join[] = ' inner join sac.avisofiltro af on af.aviid = a.aviid ';
        } elseif(!empty($filtro['estuf'])) {
            $uf = (array) $filtro['estuf'];
            $where[] = " af.estuf in ('" . implode("', '", $uf) . "') ";
            $join[] = ' inner join sac.avisofiltro af on af.aviid = a.aviid ';
        }

        $join = implode(' ', array_unique($join));
        
        $sql = "select  distinct a.aviid, a.aviid as codigo, a.avititulo,
                        to_char(avidatainclusao, 'DD/MM/YYYY HH24:MI:SS') as avidatainclusao,
                        to_char(avidatainicio, 'DD/MM/YYYY') as avidatainicio,
                        to_char(avidatafim, 'DD/MM/YYYY') as avidatafim,
                        case
                            when avitipoexibicao = 'M' then 'Modal'
                            when avitipoexibicao = 'T' then 'Tarja na tela'
                            when avitipoexibicao = 'A' then 'Somente Aviso'
                            else avitipoexibicao
                        end as avitipoexibicao
                from sac.aviso a
                    $join
                where " . implode(' and ', $where) . "
                ";
        return $sql;
    }
}//end Class
?>