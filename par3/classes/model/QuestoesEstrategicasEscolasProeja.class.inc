<?php
/**
 * Classe de mapeamento da entidade par3.questoesestrategicas_escolas_proeja
 *
 * @category Class
 * @package  A1
 * @author   JAIR FORO <jairsantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */



/**
 * Par3_Model_QuestoesEstrategicasEscolasProeja
 *
 * @category Class
 * @package  A1
 * @author    JAIR FORO <jairsantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Par3_Model_QuestoesEstrategicasEscolasProeja extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.questoesestrategicas_escolas_proeja';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'qeeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'qrpid' => array('tabela' => 'questionario.questionarioresposta', 'pk' => 'qrpid'),
        'perid' => array('tabela' => 'questionario.pergunta', 'pk' => 'perid'),
        'entid' => array('tabela' => 'entidade.entidade', 'pk' => 'entid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qeeid' => null,
        'qrpid' => null,
        'perid' => null,
        'co_entidade' => null,
//         'entid' => null,
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'qeeid' => array(  'Digits'  ),
            'qrpid' => array(  'Digits'  ),
            'perid' => array(  'Digits'  ),
            'co_entidade' => array(  'Digits'  ),
//             'entid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função pegarSQLLista
     * - monta SQL lista de escolas com Proeja
     */
    public function pegarSQLLista($arrPost)
    {
        $where = self::montarFiltroSQL($arrPost);

        $sql = "SELECT distinct
                	sen.co_entidade as id,
                	qee.co_entidade,
                	mun.mundescricao as descricaoMunicipio,
                	sen.co_entidade as codigoescola,
                	sen.no_entidade as nomeescola
                FROM
                	educacenso_2015.ts_censo_basico_escola sen
                INNER JOIN territorios.estado 			            est ON est.estcod::smallint  = sen.co_uf
                INNER JOIN territorios.municipio                    mun ON mun.muncod::bigint 	 = sen.co_municipio
                LEFT  JOIN par3.questoesestrategicas_escolas_proeja qee ON qee.co_entidade 	    = sen.co_entidade AND qee.qrpid = {$arrPost['qrpid']} AND perid = {$arrPost['perid']}
                LEFT  JOIN questionario.questionarioresposta        qrp ON qrp.qrpid     	= qee.qrpid
                LEFT  JOIN par3.questionario_unidade                qun ON qun.qrpid = qrp.qrpid AND qun.queid IN (".QUESTIONARIO_QUESTOES_ESTRATEGICAS_EST.",".QUESTIONARIO_QUESTOES_ESTRATEGICAS_MUN.",".QUESTIONARIO_QUESTOES_ESTRATEGICAS_DF.")
                WHERE
                    ".implode(' AND ', $where)."
                    AND
                    CASE WHEN ((select itrid from par3.instrumentounidade where inuid = {$arrPost['inuid']}) = 2) THEN
                        tp_dependencia = 3
                    WHEN ((select itrid from par3.instrumentounidade where inuid = {$arrPost['inuid']}) = 1) THEN
                        tp_dependencia = 2
                    ELSE
                        1 = 1
                    END
                ORDER BY
                    mun.mundescricao, nomeescola";

        return $sql;

    }//end pegarSQLLista()


   /**
     * Montar filtro da SQL
     */
    public function montarFiltroSQL($arrPost)
    {
        $where = array();

        $modelUnidade = new Par3_Model_InstrumentoUnidade($arrPost['inuid']);

        if ($modelUnidade->itrid === '2') {
            $where[] = "sen.co_municipio = {$modelUnidade->muncod}";
        } else {
            $where[] = "est.estuf = '{$modelUnidade->estuf}'";
        }

        return $where;

    }//end montarFiltroSQL()


   /**
     * Função retornarQtdEscolas
     * - montar filtro da SQL
     *
     */
    public function retornarQtdEscolasProeja($qrpid, $perid)
    {
        $sql = "SELECT count(DISTINCT co_entidade) FROM {$this->stNomeTabela}
                WHERE qrpid = $qrpid AND perid = $perid";

        $qtd = $this->pegaUm($sql);

        return $qtd>0 ? $qtd : 0;

    }//end retornarQtdEscolas()


   /**
     * Função excluirPorEntid
     * - excluir escola
     *
     */
    public function excluirPorEntid($arrPost)
    {
        $sql = "DELETE FROM {$this->stNomeTabela}
                WHERE
                    qrpid = {$arrPost['qrpid']}
                    AND perid = {$arrPost['perid']}
                    AND co_entidade = {$arrPost['co_entidade']}";

        $qtd = $this->executar($sql);

    }//end excluirPorEntid()

    /**
     * Retorna escola selecionadas no questionario
     * @param $qrpid
     * @param $perid
     * @return array|mixed|NULL
     */
    public function retornaSelecao($arrPost)
    {
        $strSQL = <<<DML
            SELECT co_entidade FROM {$this->stNomeTabela}
            WHERE qrpid = {$arrPost['qrpid']} and perid = {$arrPost['perid']}
DML;
        return $this->carregar($strSQL);
    }

    /**
     * Deleta por questionario e periodo
     * @param $arrPost
     */
    public function excluirPorQuestionarioPeriodo($arrPost)
    {
        $sql = "DELETE FROM {$this->stNomeTabela}
                WHERE
                    qrpid = {$arrPost['qrpid']}
                    AND perid = {$arrPost['perid']}";

        $qtd = $this->executar($sql);
    }

}//end Class
?>