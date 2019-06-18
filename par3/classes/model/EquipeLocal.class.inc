<?php
/**
 * Classe de modelo do EquipeLocal
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-10-2015
 * @link     no link
 */


/**
 * Par3_Model_EquipeLocal
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-10-2015
 * @link     no link
 */
class Par3_Model_EquipeLocal extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.equipelocal';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = [
        'eloid',
    ];

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = [
        'ecaid' => ['tabela' => 'par3.equipelocal_cargo', 'pk' => 'ecaid'],
        'entid' => ['tabela' => 'par3.instrumentounidade_entidade', 'pk' => 'entid'],
    ];

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = [
        'eloid' => null,
        'entid' => null,
        'ecaid' => null,
        'ecadsc' => null,
        'elostatus' => null,
        'elodtcriacao' => null,
        'elodtexclusao' => null,
        'elosegmentooutros' => null,
        'elocargooutros' => null
    ];

    /**
     * @param array $arrPost
     * @return string
     */
    public function pegarSQLLista(array $arrPost = [])
    {
        $sql = sprintf('
            SELECT
              elo.eloid as id,
              ent.entcpf,
              ent.entnome,
              eca.ecadsc,
              ese.esedsc,
              ent.entemail
            FROM 
              par3.equipelocal elo
              INNER JOIN par3.equipelocal_cargo eca ON eca.ecaid = elo.ecaid
              INNER JOIN par3.equipelocal_seguimento ese ON ese.eseid = eca.eseid
              INNER JOIN par3.instrumentounidade_entidade ent ON ent.entid = elo.entid
            WHERE
              ent.inuid = %d
              AND ent.tenid = %d
            ',
            $arrPost['inuid'],
            Par3_Model_InstrumentoUnidadeEntidade::EQUIPE
        );

        if (array_key_exists('elostatus', $arrPost) && !empty($arrPost['elostatus'])) {
            $sql .= sprintf(' AND elo.elostatus = \'%s\'', $arrPost['elostatus']);
        }

        return $sql;
    }

    /**
     * @param $arrPost
     * @return array|mixed|NULL
     */
    public function carregaArrayEquipe(array $arrPost = [])
    {
        $sql = $this->pegarSQLLista($arrPost);
        return $this->carregar($sql, 0, 30);
    }

    /**
     * @param $entcpf
     * @param $inuid
     * @return array|mixed|NULL
     */
    public function carregaIntegrante($entcpf, $inuid)
    {
    	$sql = sprintf('
            SELECT
              elo.eloid as id,
              ent.entcpf,
              ent.entnome,
              eca.ecadsc,
              ese.esedsc,
              ent.entemail
            FROM 
              par3.equipelocal elo
              INNER JOIN par3.equipelocal_cargo eca ON eca.ecaid = elo.ecaid
              INNER JOIN par3.equipelocal_seguimento ese ON ese.eseid = eca.eseid
              INNER JOIN par3.instrumentounidade_entidade ent ON ent.entid = elo.entid
            WHERE
              ent.entcpf = \'%s\'
              AND ent.inuid = %d 
              AND elostatus = \'A\'',
            $entcpf,
            $inuid
        );

    	return $this->carregar($sql);
    }

    /**
     * @param $entemail
     * @param $inuid
     * @return array|mixed|NULL
     */
    public function carregaIntegrantePorEmail($entemail, $inuid)
    {
    	$sql = sprintf('
            SELECT
              elo.eloid as id,
              ent.entcpf,
              ent.entnome,
              eca.ecadsc,
              ese.esedsc,
              ent.entemail
            FROM 
              par3.equipelocal elo
              INNER JOIN par3.equipelocal_cargo eca ON eca.ecaid = elo.ecaid
              INNER JOIN par3.equipelocal_seguimento ese ON ese.eseid = eca.eseid
              INNER JOIN par3.instrumentounidade_entidade ent ON ent.entid = elo.entid
            WHERE
              ent.entemail = \'%s\'
              AND ent.inuid = %d
              AND elo.elostatus = \'A\'',
            $entemail,
            $inuid
        );

    	return $this->carregar($sql);
    }
}