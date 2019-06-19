<?php
/**
 * Classe de mapeamento da entidade sac.avisodestinatario
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */



/**
 * Sac_ModelAvisodestinatario
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */
class Sac_Model_AvisoDestinatario extends Modelo
{
    const K_STATUS_LIDO     = 'L';
    const K_STATUS_NAO_LIDO = 'N';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.avisodestinatario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'avdid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'avdid' => null,
        'usucpf' => null,
        'avdstatus' => null,
        'avddataleitura' => null,
        'aviid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'avdid' => array(  'Digits'  ),
            'usucpf' => array(  new Zend_Validate_StringLength(array('max' => 11))  ),
            'avdstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'avddataleitura' => array( 'allowEmpty' => true ),
        );
    }//end getCamposValidacao($dados)

    public function recuperarUsuarios($dados, $sisid = null)
    {
        $sisid =  $sisid ? $sisid : $_SESSION['sisid'];

        $sql = "select sisdiretorio from seguranca.sistema where sisid = $sisid";
        $esquema = $this->pegaUm($sql);

        $where = array(" p.sisid = $sisid ");

        if(!empty($dados['pflcod'])){
            $where[] = " pu.pflcod in ('" . implode("', '", $dados['pflcod']) . "') ";
        }

        if(!empty($dados['muncod'])){
            $where[] = " ur.muncod in ('" . implode("', '", $dados['muncod']) . "') ";
        } elseif(!empty($dados['estuf'])) {
            $where[] = " (
                            ur.estuf in ('" . implode("', '", $dados['estuf']) . "') or
                            ur.muncod in (select muncod from territorios.municipio where estuf in ('" . implode("', '", $dados['estuf']) . "'))
                         )";
        }

        $sql = "select distinct pu.usucpf
                from seguranca.perfilusuario pu
                    inner join seguranca.perfil p on p.pflcod = pu.pflcod
                    left join {$esquema}.usuarioresponsabilidade ur on pu.usucpf = ur.usucpf and ur.rpustatus = 'A'
                where " . implode(" and ", $where) . "
                ";

        $usuarios = $this->carregar($sql);
        return $usuarios ? $usuarios : array();
    }

    public function listar($aviid)
    {
        $sql = "select  distinct u.usucpf, u.usunome,
                        ad.avdstatus,
                        case
                            when avdstatus = 'N' then 'Não Lida'
                            when avdstatus = 'L' then 'Lida'
                            else avdstatus
                        end as avdstatus,
                        to_char(ad.avddataleitura, 'DD/MM/YYYY HH24:MI:SS') as avddataleituraa
                from sac.avisodestinatario ad
                        inner join seguranca.usuario u on u.usucpf = ad.usucpf
                where aviid = $aviid
                order by ad.avdstatus, u.usunome
                ";

        return $sql;
    }


}//end Class
?>