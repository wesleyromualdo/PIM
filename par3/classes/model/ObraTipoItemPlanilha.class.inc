<?php
/**
 * Classe de mapeamento da entidade par3.ObraTipoItemPlanilha.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_ObraTipoItemPlanilha: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraTipoItemPlanilha($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraTipoItemPlanilha($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $oipid  - default: nextval('par3.obra_tipo_item_planilha_oipid_seq'::regclass)
 * @property int $unmcod
 * @property int $oipid_pai
 * @property int $tplid
 * @property string $oipdsc
 * @property string $oipordem
 * @property string $oipcodigo
 * @property string $oipregiao
 */
class Par3_Model_ObraTipoItemPlanilha extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_item_planilha';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'oipid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'tplid' => array('tabela' => 'par3.obra_tipo_versao_planilha', 'pk' => 'tplid'),
        'oipid_pai' => array('tabela' => 'par3.obra_tipo_item_planilha', 'pk' => 'oipid'),
        'unmcod' => array('tabela' => 'unidademedida', 'pk' => 'unmcod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'oipid'             => null,
        'uniid'             => null,
        'oipid_pai'         => null,
        'tplid'             => null,
        'oipdsc'            => null,
        'oipordem'          => null,
        'oipcodigo'         => null,
        'oippossui_valor'   => null,
        'oipstatus'         => null,
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function montarSQLSimples($arrPost)
    {
        $mVersaoPlanilha = new Par3_Model_ObraTipoVersaoPlanilha($arrPost['tplid']);
        $where = array("oip.oipstatus = 'A'");
        if($arrPost['tplid']) $where[] = "oip.tplid = '{$arrPost['tplid']}'";

        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        if($mVersaoPlanilha->tpltipo_regiao != 'N'){
            $sql = "SELECT
                        oipid||'_item' as id,
                        ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                        oipid as codigo,
                        oipordem||' '||oipdsc as descricao,
                        uni.unidsc,
                        oip.oipstatus
                    FROM par3.obra_tipo_item_planilha oip
                    LEFT JOIN par3.unidade_medida  uni ON uni.uniid = oip.uniid
                    WHERE
                        ".implode(' AND ', $where)."
                    ORDER BY
                    	oipcodigo";
        }else{
            $sql = "SELECT
                        oip.oipid||'_item' as id,
                        ROW_NUMBER() OVER () as qtd,
                        oip.oipid as codigo,
                        oipordem||' '||oipdsc as descricao,
                        uni.unidsc,
                        oip.oipstatus,
                        oiv.oivqtd,
                        CASE WHEN oiv.oivqtd_editavel = 'S'
                        	THEN 'Sim'
                        	ELSE 'Não'
                        END oivqtd_editavel,
                        oiv.oivvalor,
                        CASE WHEN oiv.oivvalor_editavel = 'S'
                        	THEN 'Sim'
                        	ELSE 'Não'
                        END oivvalor_editavel
                    FROM par3.obra_tipo_item_planilha oip
                    LEFT JOIN par3.obra_tipo_item_planilha_valores     oiv ON oiv.oipid = oip.oipid
                    LEFT JOIN par3.unidade_medida                      uni ON uni.uniid = oip.uniid
                    WHERE
                        ".implode(' AND ', $where)."
                    ORDER BY
                    	oipcodigo";
        }

        return $sql;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'oipid' => array('Digits'),
            'unmcod' => array('allowEmpty' => true,'Digits'),
            'oipid_pai' => array('allowEmpty' => true,'Digits'),
            'tplid' => array('allowEmpty' => true,'Digits'),
            'oipdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'oipordem' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'oipcodigo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'oipregiao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
        );
    }

    public function salvarItens($arrPost, $dados, $estufs = array(), $muncods = array())
    {
        $tplid = $arrPost['tplid'];

        foreach ($dados as $linha) {

            unset($n);
            $columns = explode(";", $linha);
            if ($columns[1] != '') {

                $oipordem           = $columns[0];

                $arr                = explode('.', $oipordem);
                foreach ($arr as $arrN) {
                    if ($arrN !== '0') {
                        $l = strlen(trim($arrN));
                        if ($l == 1) {
                            $n .= '00' . trim($arrN);
                        } elseif ($l == 2) {
                            $n .= '0' . trim($arrN);
                        } else {
                            $n .= trim($arrN);
                        }
                    }
                }
                $oipcodigo          = $n;

                $descricao          = $columns[1];
                $oipdsc             = str_replace(Array('\''), Array('\'\''), $descricao);

                $uniid              = $columns[2] ? $columns[2] : 'NULL';

                $quantidade         = $columns[3];
                $quantidade         = str_replace('.', '', $quantidade);
                $oivqtd             = trim(str_replace(',', '.', $quantidade));

                $oivqtd_editavel    = trim($columns[4]);

                $valor              = $columns[5];
                $valor              = str_replace('.', '', $valor);
                $oivvalor           = trim(str_replace(',', '.', $valor));

                $oippossui_valor    = $oivvalor != '' ? 'TRUE' : 'FALSE';

                $oivvalor_editavel  = trim($columns[6]);

                $sql = "SELECT oipid FROM par3.obra_tipo_item_planilha WHERE oipordem = '$oipordem' AND tplid = $tplid;";

                $oipid = $this->pegaUm($sql);

                if (!$oipid) {
                    $codpai = substr($oipordem, 0, -3);
                    if ($codpai != '') {
                        $sql = "SELECT oipid FROM par3.obra_tipo_item_planilha WHERE tplid = {$tplid} AND oipcodigo = '".$codpai."'";
                        $oipid_pai = $this->pegaUm($sql);
                    }
                    $oipid_pai = $oipid_pai ? $oipid_pai : 'NULL';

                    $sql = "INSERT INTO par3.obra_tipo_item_planilha
                                (uniid, oipid_pai, tplid, oipdsc, oipordem, oipcodigo, oippossui_valor, oipstatus)
                            VALUES
                                ($uniid, $oipid_pai, $tplid, '$oipdsc', '$oipordem', '$oipcodigo', $oippossui_valor, 'A')
                            RETURNING
                                oipid;";

                    $oipid = $this->pegaUm($sql);
                    $this->commit();
                }
                if ($oippossui_valor == 'TRUE') {

                    $ctlItensValos = new Par3_Controller_ObraTipoItemPlanilhaValores();
                    $ctlItensValos->salvarValoresItem($oipid, $oivqtd, $oivqtd_editavel, $oivvalor, $oivvalor_editavel, $estufs, $muncods);
                }
                $this->commit();
            }
        }
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

}
