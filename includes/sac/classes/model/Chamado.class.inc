<?php
/**
 * Classe de mapeamento da entidade sac.chamado
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */



/**
 * Sac__ModelChamado
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */
class Sac_Model_Chamado extends Modelo
{
    const K_STATUS_ABERTO                   = 'AB';
    const K_STATUS_AGUARDANDO_VALIDACAO     = 'AV';
    const K_STATUS_FINALIZADO_POSITIVO      = 'FP';
    const K_STATUS_FINALIZADO_NEGATIVO      = 'FN';
    const K_STATUS_FINALIZADO_AUTOMATICO    = 'FA';
    const K_STATUS_AGUARDANDO_USUARIO       = 'AU';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.chamado';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'chaid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'catid' => array('tabela' => 'sac.categoria', 'pk' => 'catid'),
        'areid' => array('tabela' => 'sac.area', 'pk' => 'areid'),
        'sisid' => array('tabela' => 'sistema', 'pk' => 'sisid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'chaid' => null,
        'chadsc' => null,
        'chadataabertura' => null,
        'usucpfabertura' => null,
        'chastatus' => null,
        'sisid' => null,
        'areid' => null,
        'chaareaoutro' => null,
        'catid' => null,
        'chacategoriaoutro' => null,
        'chaurlorigem' => null,
        'inuid' => null,
        'chaidpai' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'chaid' => array(  'Digits'  ),
            'chadsc' => array(  new Zend_Validate_StringLength(array('max' => 4000))  ),
            'chadataabertura' => array(  ),
            'usucpfabertura' => array(  new Zend_Validate_StringLength(array('max' => 11))  ),
            'chastatus' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'sisid' => array(  'Digits'  ),
            'areid' => array(  'Digits'  ),
            'chaareaoutro' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
            'catid' => array(  'Digits'  ),
            'chacategoriaoutro' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
        );
    }//end getCamposValidacao($dados)

    public function recuperarChamado()
    {
        if($this->chaid){
            $sql = "select c.chadsc, to_char(chadataabertura, 'DD/MM/YYYY HH:mi:ss') as chadataabertura,
                           a.aredsc, cat.catdsc, u.usunome, u.usuemail,
                           case
                              when coalesce(u.usufoneddd, '') != '' then '(' || u.usufoneddd || ') '
                           end as usufoneddd,
                           case
                              when c.chastatus = 'AB' then 'Aguardando Retorno do Suporte'
                              when c.chastatus = 'AV' then 'Aguardando Validação do Usuário'
                              when c.chastatus = 'AU' then 'Aguardando Retorno do Usuário'
                              when c.chastatus = 'FP' then 'Finalizado Positivo'
                              when c.chastatus = 'FN' then 'Finalizado Negativo'
                              when c.chastatus = 'FA' then 'Finalizado Automaticamente'
                              else c.chastatus
                           end as status,
                           u.usufonenum, u.usufuncao, iu.estuf, iu.inudescricao, ca.cardsc
                    from sac.chamado c
                        inner join sac.area a on a.areid = c.areid
                        inner join sac.categoria cat on cat.catid = c.catid
                        inner join seguranca.usuario u on u.usucpf = c.usucpfabertura
                        left join par3.instrumentounidade iu on iu.inuid = c.inuid
                        left join public.cargo ca on ca.carid = u.carid
                    where chaid = {$this->chaid}
                    ";
            return $this->pegaLinha($sql);
        }

        return array();
    }

    public function listar($sisid, $filtro = array())
    {
        $where[] = "c.sisid = $sisid";
        $join = '';

        if(!empty($filtro['chaid'])){
            $where[] = " c.chaid = '{$filtro['chaid']}' ";
        }
        if(!empty($filtro['chadataabertura'])){
            $where[] = " TO_CHAR(c.chadataabertura, 'DD/MM/YYYY') = '" . $filtro['chadataabertura'] . "' ";
        }
        if(!empty($filtro['cpf'])){
            $where[] = " c.usucpfabertura = '{$filtro['cpf']}' ";
        }
        if(!empty($filtro['inuid'])){
            $where[] = " c.inuid = '{$filtro['inuid']}' ";
        }
        if(!empty($filtro['chastatus'])){
            $status = (array) $filtro['chastatus'];
            $where[] = " c.chastatus in ('" . implode("', '", $status) . "') ";
        }
        if(!empty($filtro['catid'])){
            $status = (array) $filtro['catid'];
            $where[] = " c.catid in ('" . implode("', '", $status) . "') ";
        }
        if(!empty($filtro['areid'])){
            $status = (array) $filtro['areid'];
            $where[] = " c.areid in ('" . implode("', '", $status) . "') ";
        }
        if(!empty($filtro['estuf'])){
            $uf = (array) $filtro['estuf'];
            $where[] = " iu.estuf in ('" . implode("', '", $uf) . "') ";
        }
        if(!empty($filtro['muncod'])){
            $muncod = (array) $filtro['muncod'];
            $where[] = " iu.muncod in ('" . implode("', '", $muncod) . "') ";
        }
        if(!empty($filtro['usuario'])){
            $status = (array) $filtro['usuario'];
            $where[] = " (
                            u.usunome ilike '%{$filtro['usuario']}%' or
                            uh.usunome ilike '%{$filtro['usuario']}%'
                         )
            ";
            $join .= ' inner join sac.chamadohistorico ch on ch.chaid = c.chaid
                       inner join seguranca.usuario uh on uh.usucpf = ch.usucpf ';
        }

//        ver($where, $filtro);
        
        $sql = "with tmp_historico as (
                    select hicid, chaid, to_char(hicdata, 'DD/MM/YYYY HH:mi:ss') as hicdata, uch.usunome
                    from sac.chamadohistorico h
                            inner join seguranca.usuario uch on uch.usucpf = h.usucpf
                )

                select distinct c.chaid, c.chaid as codigo, substring(c.chadsc, 0, 100),
                       case
                          when c.chastatus = 'AB' then 'Aguardando Retorno do Suporte'
                          when c.chastatus = 'AV' then 'Aguardando Validação do Usuário'
                          when c.chastatus = 'AU' then 'Aguardando Retorno do Usuário'
                          when c.chastatus = 'FP' then 'Finalizado Positivo'
                          when c.chastatus = 'FN' then 'Finalizado Negativo'
                          when c.chastatus = 'FA' then 'Finalizado Automaticamente'
                          else c.chastatus
                       end as status,
                       to_char(chadataabertura, 'DD/MM/YYYY HH:mi:ss') as chadataabertura, u.usunome,
                       (select hicdata from tmp_historico h where h.chaid = c.chaid order by hicdata desc, hicid desc limit 1) datahistorico,
                          (select usunome from tmp_historico h where h.chaid = c.chaid order by hicdata desc, hicid desc limit 1) nomehistorico,
                       cat.catdsc, a.aredsc, iu.estuf || ' - ' || iu.inudescricao AS estuf
                from sac.chamado c
                    inner join sac.area a on a.areid = c.areid
                    inner join sac.categoria cat on cat.catid = c.catid
                    inner join seguranca.usuario u on u.usucpf = c.usucpfabertura
                    left join par3.instrumentounidade iu on iu.inuid = c.inuid
                    $join
                where " . implode(' and ', $where) . "
                ";
        return $sql;
    }

    public function getSituacoes()
    {
        return array(
            self::K_STATUS_ABERTO                   => 'Aguardando Retorno do Suporte',
            self::K_STATUS_AGUARDANDO_VALIDACAO     => 'Aguardando Validação do Usuário',
            self::K_STATUS_FINALIZADO_POSITIVO      => 'Finalizado Positivo',
            self::K_STATUS_FINALIZADO_NEGATIVO      => 'Finalizado Negativo',
            self::K_STATUS_FINALIZADO_AUTOMATICO    => 'Finalizado Automaticamente',
            self::K_STATUS_AGUARDANDO_USUARIO       => 'Aguardando Retorno do Usuário',
        );
    }
}//end Class
?>